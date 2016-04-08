#include Projectile

local Speed = 95;
local SpellDamage = 3;
local Size = 30;
local ManaCost = 20;

local traveledTime = 0;
local MaxTravelTime = 180;

local TravelAngle = 0;

func Initialize()
{
	
}

func InitEffect()
{
	Sound("Flash::projectileshoot", false, 100);
}

func TravelEffect(int time)
{
	var trailparticles =
	{
		Size = PV_Linear(PV_Random(10, 22),0),
		BlitMode = GFX_BLIT_Additive,
		R = 250 - (250 * time) / MaxTravelTime,
		G = PV_Random(20, 180),
		B = (250 * time) / MaxTravelTime,
	};
	
	CreateParticle("Flash", PV_Random(-2,2), PV_Random(-2,2), 0, 0, 30, trailparticles);
	
	if (time > MaxTravelTime)
	{
		TravelAngle = Angle(0,0, GetXDir(), GetYDir());
		
		Fireworks();
		
		Explode(15);
	}
}

func HitObject(obj)
{
	if(obj->~CanBeHit() == false)
		return;
		
	obj->Fling(RandomX(-7, 7), RandomX(-7,7), nil, true);
	WeaponDamage(obj, SpellDamage);
}

func HitDamage(obj)
{
	if(obj->~CanBeHit() == false)
		return;
	
	obj->AddFlashHitEffect();
	WeaponDamage(obj, SpellDamage);
}

func Hit(int xdir, int ydir)
{
	Bounce(xdir, ydir);
	Sound("Flash::Glass?");
}

func Bounce(int xdir, int ydir)
{
	var angle = Angle(0, 0, xdir, ydir);
	
	var surface = GetSurfaceVector(0, 0);
	var surface_angle = Angle(0, 0, surface[0], surface[1]);
	var angle_diff = GetTurnDirection(angle - 180, surface_angle);
	var new_angle = surface_angle + angle_diff;
	
	var speed = Distance(0, 0, xdir, ydir);
	SetXDir(Sin(new_angle, speed), 100);
	SetYDir(-Cos(new_angle, speed), 100);
}

