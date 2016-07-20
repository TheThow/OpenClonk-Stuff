#include Projectile

local SpellDamage = 22;
local Speed = 60;
local firetrailparticles;
local ParticleSize = 10;

func InitEffect()
{
	Sound("Fire::Fireball", false, 100);
	Sound("Fire::FuseLoop", false, 50, nil, 1);
	SetLightRange(30, 70);
	SetLightColor(RGB(255, 100, 0));
	
	if (shooter.ChampType == ComboMan)
	{
		Speed = 75;
		ParticleSize = 13;
		SpellDamage = 25;
		SetVelocity(Angle(0,0,GetXDir(), GetYDir(), 10), Speed, 10);
	}

	var lightparticle =
	{
		R = 255,
		G = 100,
		B = 0,
		Alpha = 40,
		Size = 50,
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Step(30, 0, 1),
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	CreateParticle("Shockwave", 0, 0, 0, 0, 0, lightparticle);
	
	firetrailparticles =
	{
		Prototype = Particles_FireTrail(),
		Size = PV_Linear(ParticleSize,0),
		BlitMode = GFX_BLIT_Additive,
	};
}

func TravelEffect(int time)
{
	CreateParticle("Fire", 0, 0, PV_Random(-7,7), PV_Random(-7,7), 10, firetrailparticles, 3);
}

func ExplosionEffect(...)
{
	return _inherited(...);
}

func HitObject(obj)
{
	//AddFireHitEffect(obj);
	return _inherited(obj);
}

func Hit()
{
	CastObjects(Flame, RandomX(1,2), RandomX(10,15));
	for(var o in FindObjects(Find_Distance(SpellDamage), Find_Func("CanBeHit", this)))
	{
		o->AddFireHitEffect();
	}
	Explode(SpellDamage);
}