/*--- Ruby ---*/

local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Plane = 480;
local graphics_index = 0;

local speed = 80;
local thrown;

local ox;
local oy;

local trailparticles;

local probability = 20;
local cluster;

func IsKnueppelItem() { return true; }

func CanBeSucked() { return true; }

func InitEffect()
{
	SetLightRange(30, 70);
	SetLightColor(RGB(255, 255, 0));
	ox=GetX();
	oy=GetY();
	
	trailparticles =
	{
		Size = PV_Linear(5,0),
		Alpha = 128,
		BlitMode = GFX_BLIT_Additive,
		R = 255,
		G = 255,
		B = 50,
	};
	
	AddEffect("Trail", this, 1, 1, this);
	AddEffect("HitCheck", this, 1,1, nil,nil, GetCrew(GetOwner()));
}

func FxTrailTimer()
{
	TravelEffect();
	if(!GetXDir() && !GetYDir() && !GetEffect("Idle", this))
		AddEffect("Idle", this, 1, 15, this);
}

public func HitObject(obj)
{
	if(obj->~CanBeHit() == false)
		return;
		
	Explosion();
}

func Initialize()
{
	graphics_index = Random(4);
	if (graphics_index) SetGraphics(Format("%d", graphics_index+1));
	
	thrown = false;
	
	return true;
}

protected func ControlUse(object clonk, int iX, int iY)
{
	Exit();
	Collectible = 0;
	var angle = Angle(0, 0, iX, iY);
	SetPosition(clonk->GetX() + Sin(angle, 5), clonk->GetY() + -Cos(angle, 5));
	SetVelocity(angle, speed);
	thrown = true;
	
	SetOwner(clonk->GetOwner());
	InitEffect();
	
	return true;
}

func TravelEffect()
{
	DrawParticleLine("Flash", 0, 0, ox-GetX(), oy-GetY(), 1, 0, 0, 10, trailparticles);
	ox=GetX();
	oy=GetY();
}

func IsReflectable()  { return thrown; }

func Hit(xdir, ydir)
{
	Sound("Items::rubberducky*", false, 50);
	
	if(!thrown)
		return;
	
	if(cluster && !Random(2))
	{
		Bounce(xdir, ydir);
		return true;
	}
	
	Explosion();

	return true;
}

func Explosion()
{
	if(!cluster)
	{
		for (var i = 0; i < 5; i++)
		{
			var obj = CreateObject(GetID(), 0, -5, GetOwner());
			obj.cluster = true;
			obj->InitEffect();
			obj->SetVelocity(RandomX(-40, 40), RandomX(speed - 10, speed + 10));
			obj.thrown = 1;
		}
	}

	ExplosionEffect(25);
	for(var o in FindObjects(Find_Distance(25), Find_Func("CanBeHit", this)))
	{
			
		var angle = Angle(GetX(), GetY(), o->GetX(), o->GetY());

		o->Fling(Sin(angle, 8), -Cos(angle, 8) - 2);
		WeaponDamage(o, 25);
	}
	
	RemoveObject();
}

func Bounce(int xdir, int ydir)
{
	var angle = Angle(0, 0, xdir, ydir);
	
	var surface = GetSurfaceVector(0, 0);
	var surface_angle = Angle(0, 0, surface[0], surface[1]);
	var angle_diff = GetTurnDirection(angle - 180, surface_angle);
	var new_angle = surface_angle + angle_diff;
	
	var speed = Distance(0, 0, xdir, ydir);
	speed = speed*3/4;
	SetXDir(Sin(new_angle, speed), 100);
	SetYDir(-Cos(new_angle, speed), 100);
}

func FxIdleStop()
{
	if(!GetXDir() && !GetYDir() && this)
		Explosion();
}