#include Projectile

local pR = 100;
local pG = 250;
local pB = 100;
local Speed = 100;
local SpellDamage = 40;
local ManaCost = 15;

local Range = 600;

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
	Sound("Objects::Weapons::Blunderbuss::GunShoot?", false, 50, nil, nil, nil, -30);
	SetLightRange(30, 70);
	SetLightColor(RGB(100, 255, 100));
	ox=GetX();
	oy=GetY();
	
	trailparticles =
	{
		Size = PV_Linear(PV_Random(3, 6),0),
		//BlitMode = GFX_BLIT_Additive,
		R = pR,
		G = pG,
		B = pB,
	};
	
	trailparticles2 =
	{
		Size = PV_Linear(9,0),
		//BlitMode = GFX_BLIT_Additive,
		R = PV_Linear(250,0),
		G = 0,
		B = 0,
		Alpha = PV_Linear(255,0),
	};
	
}

func TravelEffect(int time)
{
	CreateParticle("Flash", PV_Random(-3,3), PV_Random(-3,3), PV_Random(-7,7), PV_Random(-7,7), 10, trailparticles, 2);
	
	//CreateParticle("Flash", 0, 0, 0, 0, 10, trailparticles2, 2);
	DrawParticleLine("Flash", 0, 0, ox-GetX(), oy-GetY(), 1, 0, 0, 13, trailparticles2);
	ox=GetX();
	oy=GetY();
	
	//if (time > 48) RemoveObject();
	
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
