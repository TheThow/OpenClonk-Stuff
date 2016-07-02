#include Projectile

local pR = 0;
local pG = 200;
local pB = 0;
local Speed = 100;
local SpellDamage = 12;
local ManaCost = 18;

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
	SetLightColor(RGB(200, 215, 255));
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
		Size = PV_Linear(PV_Random(2, 4),0),
		BlitMode = GFX_BLIT_Additive,
		R = 50,
		G = 50,
		B = 50,
	};
	
}

func TravelEffect(int time)
{
	CreateParticle("Flash", PV_Random(-3,3), PV_Random(-3,3), PV_Random(-7,7), PV_Random(-7,7), 10, trailparticles, 2);
	
	//CreateParticle("Flash", 0, 0, 0, 0, 10, trailparticles2, 2);
	DrawParticleLine("Flash", 0, 0, ox-GetX(), oy-GetY(), 1, 0, 0, 10, trailparticles2);
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
	
	if (obj->GetID() == Clonk)
		AddEffect("PoisonDart", obj, 1, 100, nil, GetID());
		
	WeaponDamage(obj, SpellDamage);
	Sound("Hits::ProjectileHitLiving?", false, 50);
	HitEffect();
}

func FxPoisonDartStart(object target, proplist effect, int temporary)
{
	var dummy = CreateObject(Dummy, target->GetX(), target->GetY(), target->GetOwner());
	dummy.Visibility = VIS_Owner;
	dummy->SetAction("HangOnto", target);
	dummy.Plane = 950;
	
	var flashparticle =
	{
		Size = 200,
		R = 0,
		G = 0,
		B = 0,
		Alpha=PV_Linear(500, 0),
		Attach = ATTACH_Front | ATTACH_MoveRelative
	};
	
	for(var c = 0; c < 4; c++)
	{
		for(var i = 0; i < 360; i+=20)
		{
			var x = Sin(i, 120 + (c * 100));
			var y = -Cos(i, 120 + (c * 100));
			
			dummy->CreateParticle("Flash", x, y, 0, 0, 100, flashparticle, 1);
		}
	}
	effect.dummy = dummy;
}

func FxPoisonDartStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;

	if(effect.dummy)
		effect.dummy->RemoveObject();
}



func Hit(int xdir, int ydir)
{
	HitEffect();
}
