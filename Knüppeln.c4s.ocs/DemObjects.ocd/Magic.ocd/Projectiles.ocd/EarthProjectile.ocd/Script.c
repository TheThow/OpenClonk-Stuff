#include Projectile

local pR = 50;
local pG = 255;
local pB = 50;
local Speed = 50;
local SpellDamage = 30;
local Size = 30;
local ManaCost = 20;

func InitEffect()
{
	Sound("Fire::Fireball", false, 100);
	Sound("Fire::FuseLoop", false, 20, nil, 1);
	SetLightRange(30, 70);
	SetLightColor(RGB(200, 215, 255));
}

func TravelEffect(int time)
{
	var trailparticles =
	{
		Size = PV_Linear(PV_Random(10, 15),0),
		BlitMode = GFX_BLIT_Additive,
		R = pR,
		G = pG,
		B = pB,
	};
	
	CreateParticle("Flash", PV_Random(-5,5), PV_Random(-5,5), PV_Random(-10,10), PV_Random(-10,10), 10, trailparticles, 5);
}

func HitEffect()
{
	ShakeViewport(SpellDamage, 0, 0);
	
	var smoke =
	{
		Alpha = PV_Linear(255, 0),
		Size = Size,
		DampingX = 900, DampingY = 900,
		R = 100, G = 100, B = 100,
		Phase = PV_Random(0, 15)
	};
	CreateParticle("Smoke", 0, 0, PV_Random(-20,20), PV_Random(-20,20), 20, smoke, 25);
	
	var sphereparticle =
	{
		Alpha = PV_Linear(255, 0),
		Size = Size*2,
		R = pR,
		G = pG,
		B = pB,
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("StarSpark", 0, 0, 0, 0, 7, sphereparticle, 4);
	
	Sound("electro_explosion", false, 50);
}

func Hit()
{
	HitEffect();
	
	for(var o in FindObjects(Find_Distance(Size), Find_Func("CanBeHit")))
	{
		o->DoEnergy(-SpellDamage);
		var angle = Angle(GetX(), GetY(), o->GetX(), o->GetY());
		o->Fling(Sin(angle, 5), -Cos(angle, 5));
	}
	
	RemoveObject();
}