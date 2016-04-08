#include Projectile

local Speed = 95;
local SpellDamage = 3;
local Size = 30;
local ManaCost = 20;

//local MaxTravelTime = 180;

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
		Size = PV_Linear(PV_Random(10, 22),10),
		BlitMode = GFX_BLIT_Additive,
		//R = 250 - (250 * time) / MaxTravelTime,
		R = PV_Random(20, 180),
		G = PV_Random(20, 180),
		//B = (250 * time) / MaxTravelTime,
		B = PV_Random(20, 180),
		Alpha = PV_Linear(255,0),
	};
	
	CreateParticle("Flash", PV_Random(-2,2), PV_Random(-2,2), 0, 0, 12, trailparticles);
	
	
}

func HitObject(obj)
{
	if(obj->~CanBeHit() == false)
		return;
		
	obj->Fling(RandomX(-7, 7), RandomX(-7,7), nil, true);
	WeaponDamage(obj, SpellDamage);
	Explode(15);
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
	Fireworks();
	Explode(15);
}

