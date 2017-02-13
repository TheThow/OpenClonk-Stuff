/**
	ElectroOrb
	

	@author 
*/

local ManaCost = 30;

local SpellDamage = 1;
local Speed = 25;
local Durr = 35;

local Size = 15;

local ChargeTime = 38;
local Length = 200;
local TargetAngle = 0;
local shooter;

func Initialize()
{
	SetCategory(C4D_StaticBack);
}

func Launch(object clonk, int x, int y)
{
	SetController(clonk->GetOwner());
	TargetAngle = Angle(0, 0, x, y);
	shooter = clonk;
	
	SoundAt("LaserMan::LightStar", nil, nil, nil, nil, nil, -200);
	
	
	this.particle_stars = 
	{
		Stretch = PV_Random(15000, 20000),
		Size = PV_Linear(2, 0),
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
	
	var from_x = shooter->GetX();
	var from_y = shooter->GetY();
	var to_x   = from_x + Sin(TargetAngle, Length);
	var to_y   = from_y - Cos(TargetAngle, Length);
	//to_x = BoundBy(to_x, 5, LandscapeWidth() - 5);
	//to_y = BoundBy(to_y, 10, LandscapeHeight() - 10); 
	
	var last_x = from_x;
	var last_y = from_y;
		
	var point  = PathFree2(from_x, from_y, to_x, to_y);
	
	if (point)
	{
		to_x = point[0];
		to_y = point[1];
	}
	var len = Distance(from_x, from_y, to_x, to_y);
	
	// Try a few positions around the target.
	shooter->SetPosition(to_x, to_y, true);
	if (shooter->Stuck()) shooter->SetPosition(to_x, to_y - 10, true);
	// Still stuck? Then try to project the shooter towards the point.
	if (shooter->Stuck())
	{
		for (var distance = 0; distance < len; ++distance)
		{
			var x_ = from_x + Sin(TargetAngle, distance);
			var y_ = from_y - Cos(TargetAngle, distance);
			shooter->SetPosition(x_, y_, true);
			if (shooter->Stuck())
			{
				shooter->SetPosition(last_x, last_y);
				break;
			}
			last_x = x_;
			last_y = y_;
		} 
	}
	shooter->CreateParticle("StarSpark", PV_Random(-10, 10), PV_Random(-10, 10), 0, 0, 30, this.particle_stars, 50);
	this->Call(LaserRay.DoTheLaser, 0, 0, Sin(TargetAngle, Length), -Cos(TargetAngle, Length), TargetAngle, Length, false, false);
	RemoveObject();
}

public func HitObject(obj)
{
	if(obj->~CanBeHit(this) == false)
		return;
	AddEffect("LaserHit", obj, 20, 1, nil, LaserMan);
	obj->Fling(0, -4, nil, true);
	WeaponDamage(obj, SpellDamage);
}

