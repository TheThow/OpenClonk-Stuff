/**
	Light Ray
	

	@author 
*/

local ManaCost = 25;

local SpellDamage = 15;
local Speed = 25;
local Durr = 35;

local Size = 15;

local Name = "$Name$";
local Description = "$Description$";

local ChargeDuration = 20;

local MaxLength = 200;
local TargetAngle = 0;
local shooter;

func Initialize()
{
	SetCategory(C4D_StaticBack);
}

func Launch(object clonk, int x, int y)
{
	shooter = clonk;
	SetController(clonk->GetOwner());
	TargetAngle = Angle(0, 0, x, y);
	clonk->Charge(this, "ChargeStop", ChargeDuration, {});
	
	AddEffect("LightRay", this, 1, 2, this, nil);
	Sound("Structures::FanLoop", nil, nil, nil, 1, nil, 200);
	
	this.particle_stars = 
	{
		Stretch = PV_Random(15000, 20000),
		Size = PV_Linear(4, 0),
		Alpha = PV_Random(255,0,3),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Random(0, 360),
		G = 0, B = 0
	};
	
	this.particle_ray = 
	{
		Size = 16,
		Alpha = PV_Linear(255,0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = TargetAngle,
		G = 0, B = 0
	};
}

func ChargeInterrupted()
{
	Remove();
}

func Remove()
{
	RemoveObject();
}

func ChargeStop()
{
	var from_x = 0;
	var from_y = 0;
	var to_x   = from_x + Sin(TargetAngle, MaxLength);
	var to_y   = from_y - Cos(TargetAngle, MaxLength);
	DoTheLaser(from_x, from_y, to_x, to_y, TargetAngle, MaxLength, false, true);
	Remove();
}

public func HitObject(obj)
{
	if(obj->~CanBeHit() == false)
		return;
	obj->Fling(0, -2, nil, true);
	obj->DoEnergy(-SpellDamage, false, nil, GetController());
}

func FxLightRayTimer(target, fx, time)
{
	if (!shooter) return -1;
	
	if (time < ChargeDuration - 2)
	{
		var radius = 2 * (ChargeDuration - time);
		for (var a = 0; a < 360; a += RandomX(20, 40))
		{
			var x = Sin(a + RandomX(-5, 5), radius);
			var y = -Cos(a + RandomX(-5, 5), radius);
			CreateParticle("StarSpark", x, y, -x/2, -y/2, 10, this.particle_stars, 4);
		}
		return FX_OK;
	}
}

func DoTheLaser(from_x, from_y, to_x, to_y, angle, max_length, no_bounce, stop_at_target)
{
	no_bounce = no_bounce ?? false;
	
	
	var len = max_length;
	var object_hit = nil;
	
	if (stop_at_target)
	{
		var object_hit = FindObject(Find_OnLine(from_x, from_y, to_x, to_y), Find_OCF(OCF_Alive), Find_Exclude(this.shooter), Find_NoContainer(), Find_Func("CanBeHit"));
		if (object_hit)
		{
			to_x = object_hit->GetX() - GetX();
			to_y = object_hit->GetY() - GetY();
		}
	}
	
	if (!object_hit)
	{
		var point  = PathFree2(GetX() + from_x, GetY() + from_y, GetX() + to_x, GetY() + to_y);
		if (point)
		{
			to_x = point[0] - GetX();
			to_y = point[1] - GetY();
			len = Distance(from_x, from_y, to_x, to_y);
			
			var remaining = max_length - len;
			if (!no_bounce && (remaining > 10) && (remaining < max_length))
			{
				var surface = GetSurfaceVector(to_x, to_y);
				var surface_angle = Angle(0, 0, surface[0], surface[1]);
				var angle_diff = GetTurnDirection(angle - 180, surface_angle);
				var new_angle = surface_angle + angle_diff;
				var new_to_x = to_x + Sin(new_angle, remaining);
				var new_to_y = to_y - Cos(new_angle, remaining);
				DoTheLaser(to_x + surface[0], to_y + surface[1], new_to_x, new_to_y, new_angle, remaining, false, stop_at_target);
			}
		}
	}
		
	this.particle_ray.Rotation = angle;
	
	CreateParticle("StarSpark", from_x, from_y, 0, 0, 10, this.particle_stars, 2);
	CreateParticle("StarSpark", to_x, to_y, 0, 0, 10, this.particle_stars, 10);
	DrawParticleLine("RaySpark", from_x, from_y, to_x, to_y, 4, 0, 0, 10, this.particle_ray);
	
	for (var obj in FindObjects(Find_OnLine(from_x, from_y, to_x, to_y), Find_OCF(OCF_Alive), Find_Exclude(this.shooter), Find_NoContainer()))
	{
		CreateParticle("StarSpark", AbsX(obj->GetX()), AbsY(obj->GetY()), 0, 0, 10, this.particle_stars, 2);
		this->HitObject(obj);
	}
}

