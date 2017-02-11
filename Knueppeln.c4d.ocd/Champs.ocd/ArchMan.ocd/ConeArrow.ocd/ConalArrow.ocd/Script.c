#include Projectile

local pR = 150;
local pG = 150;
local pB = 250;
local Speed = 130;
local SpellDamage = 9;

local counter = 0;

local ox;
local oy;

local trailparticles;
local trailparticles2;
func Initialize()
{
	SetAction("Travel");
}

func Construction()
{
	this.ActMap = new Projectile.ActMap { Travel = new Projectile.ActMap.Travel { Speed = 10000 }};
}

func Launch(object clonk, int x, int y, int anglediff)
{
	var angle = Angle(0,0,x,y, 100) + (anglediff * 10);
	shooter = clonk;
	SetVelocity(angle, Speed * 100, 100, 1000);
	SetController(clonk->GetController());
	AddEffect("HitCheck", this, 1,1, nil,nil, clonk);
	AddEffect("TheEffect", this, 1, 1, this, Projectile);
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
		G = 100,
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
	
	if (time > 15) RemoveObject();
	
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
