#include Projectile

local pR = 255;
local pG = 50;
local pB = 50;
local Speed = 75;
local SpellDamage = 1000;
local Size = 20;
local ManaCost = 15;

func InitEffect()
{
	Sound("electro_shot", false, 100);
	Sound("electro_travel", false, 50, nil, 1);
	SetLightRange(30, 70);
	SetLightColor(RGB(255, 0, 0));

	var lightparticle =
	{
		Alpha = 30,
		Size = 30,
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Step(30, 0, 1),
		Attach = ATTACH_Back | ATTACH_MoveRelative,
		R = pR,
		G = pG,
		B = pB,
		
	};
	CreateParticle("Flash", 0, 0, 0, 0, 0, lightparticle);
	
	var lightparticle2 =
	{
		Alpha = 255,
		Size = 8,
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Step(30, 0, 1),
		Attach = ATTACH_Back | ATTACH_MoveRelative,
		R = pR,
		G = pG,
		B = pB,
		
	};
	CreateParticle("Shockwave", 0, 0, 0, 0, 0, lightparticle2, 2);
}

func TravelEffect(int time)
{
	var trailparticles =
	{
		Prototype = Particles_ElectroSpark2(),
		Size = PV_Linear(PV_Random(5,15),0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = Angle(0,0,GetXDir(), GetYDir()),
		R = pR,
		G = pG,
		B = pB,
	};
	
	var angle = Angle(0,0, GetXDir(), GetYDir()) + 180;
	
	CreateParticle("Lightning", Sin(angle, 5), -Cos(angle, 5), PV_Random(-10,10), PV_Random(-10,10), 10, trailparticles, 5);
}

func HitEffect()
{
	var particles =
	{
		Prototype = Particles_Glimmer(),
		R = pR,
		G = pG,
		B = pB,
		Alpha = 255,
		Size = PV_Linear(10, 0),
		OnCollision = PC_Bounce(),
	};
	CreateParticle("StarSpark", 0, 0, PV_Random(-60,60), PV_Random(-60, 60), 25, particles, 20);
	
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
		WeaponDamage(o, SpellDamage);
	}
	
	RemoveObject();
}