#include Projectile

local pR = 100;
local pG = 100;
local pB = 250;
local Speed = 100;
local SpellDamage = 12;
local ManaCost = 15;

local counter = 0;

local ox;
local oy;

local trailparticles;
local trailparticles2;
func Initialize()
{
}

func InitEffect()
{
	Sound("Objects::Arrow::Shoot?", false, 50);
	SetLightRange(30, 70);
	SetLightColor(RGB(100, 100, 255));
	ox=GetX();
	oy=GetY();
	
	trailparticles =
	{
		Size = PV_Linear(PV_Random(2, 3),0),
		BlitMode = GFX_BLIT_Additive,
		R = pR,
		G = pG,
		B = pB,
	};
	
	trailparticles2 =
	{
		Size = PV_Linear(5,0),
		BlitMode = GFX_BLIT_Additive,
		R = 50,
		G = 50,
		B = 250,
	};
	
}

func TravelEffect(int time)
{
	//CreateParticle("Flash", PV_Random(-3,3), PV_Random(-3,3), PV_Random(-7,7), PV_Random(-7,7), 10, trailparticles, 2);
	
	//CreateParticle("Flash", 0, 0, 0, 0, 10, trailparticles2, 2);
	DrawParticleLine("Flash", 0, 0, ox-GetX(), oy-GetY(), 1, 0, 0, 7, trailparticles2);
	ox=GetX();
	oy=GetY();
	
}

func HitEffect()
{
	var sphereparticle =
	{
		Alpha = PV_Linear(255, 0),
		Size = 10,
		R = pR,
		G = pG,
		B = pB,
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("StarSpark", 0, 0, 0, 0, 7, sphereparticle, 4);
	
	RemoveObject();
}

func HitObject(obj)
{
	if(obj->~CanBeHit() == false)
		return;
		
	WeaponDamage(obj, SpellDamage);
	Sound("Hits::ProjectileHitLiving?");
	HitEffect();
}



func Hit(int xdir, int ydir)
{
	HitEffect();
}
