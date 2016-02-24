#include Projectile

local pR = 255;
local pG = 150;
local pB = 50;
local Speed = 75;
local SpellDamage = 10;
local MarkDamage = 20;
local Size = 15;
local ManaCost = 15;
local ox;
local oy;

local trailparticles;

func InitEffect()
{
	Sound("time_shot", false, 30);
	SetLightRange(30, 70);
	SetLightColor(RGB(200, 215, 255));
	
	ox = GetX();
	oy = GetY();
	
	trailparticles =
	{
		Size = PV_Linear(5,0),
		BlitMode = GFX_BLIT_Additive,
		R = PV_Linear(255, 150),
		G = PV_Linear(150, 50),
		B = PV_Linear(100, 0),
	};
}

func TravelEffect(int time)
{
	DrawParticleLine("Flash", 0, 0, ox - GetX(), oy - GetY(), 2, 0, 0, 10, trailparticles);
	ox = GetX();
	oy = GetY();
}

func HitEffect()
{
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
	Sound("time_hit", false, 50);
	
}

func Hit()
{
	HitEffect();
	
	for(var o in FindObjects(Find_Distance(Size), Find_Func("CanBeHit")))
	{
		o->AddTimeHitEffect();
		var flag = false;
		
		if(GetEffect("TimeMark", o))
		{
			MarkHit();
			flag = true;
			RemoveEffect("TimeMark", o);
		}
		
		if(!o)
			continue;
			
		//var angle = Angle(GetX(), GetY(), o->GetX(), o->GetY());
		//o->SetVelocity(angle, 10);
		
		if(!flag)
		{
			AddEffect("TimeMark", o, 1, 1, nil, GetID());
		}
		
		WeaponDamage(o, SpellDamage);
	}
	
	RemoveObject();
}

func FxTimeMarkStart(target, effect)
{
	effect.props = {
		Size = 20,
		BlitMode = GFX_BLIT_Additive,
		R = 255,
		G = 165,
		B = 50,
		Alpha=50,
		Attach=ATTACH_Front,
	};
}

func FxTimeMarkTimer(target, effect, time)
{	
	if(time > 80)
		return -1;
		
	target->CreateParticle("Flash", 0, 0, 0, 0, 1, effect.props, 2);
}

func MarkHit()
{
	var particles =
	{
		Prototype = Particles_Glimmer(),
		R = pR,
		G = pG,
		B = pB,
		Alpha = 255,
		Size = PV_Linear(5, 0),
		OnCollision = PC_Bounce(),
	};
	CreateParticle("StarSpark", 0, 0, PV_Random(-60,60), PV_Random(-60, 60), 55, particles, 20);
	
	var sphereparticle =
	{
		Alpha = PV_Linear(255, 0),
		Size = Size*4,
		R = pR,
		G = pG,
		B = pB,
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("StarSpark", 0, 0, 0, 0, 7, sphereparticle, 4);
	
	for(var i = 0; i < 8; i++)
	{
		this->CreateTimeTrail(20, RandomX(-140, 140), 0, 0, nil);
	}
	
	for(var o in FindObjects(Find_Distance(Size*2), Find_Func("CanBeHit")))
	{
		o->AddTimeHitEffect();
		var angle = Angle(GetX(), GetY(), o->GetX(), o->GetY());
		o->Fling(Sin(angle, 5), -Cos(angle, 5));
		WeaponDamage(o, MarkDamage);
	}
	
	Sound("mark_hit", false, 80);
}