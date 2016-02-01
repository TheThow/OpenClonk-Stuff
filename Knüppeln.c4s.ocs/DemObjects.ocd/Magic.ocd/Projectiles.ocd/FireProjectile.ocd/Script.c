#include Projectile

local SpellDamage = 20;

func InitEffect()
{
	Sound("Fire::Fireball", false, 100);
	Sound("Fire::FuseLoop", false, 20, nil, 1);
	SetLightRange(30, 70);
	SetLightColor(RGB(255, 100, 0));

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
}

func TravelEffect(int time)
{
	var firetrailparticles =
	{
		Prototype = Particles_FireTrail(),
		Size = PV_Linear(10,0),
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("Fire", 0, 0, PV_Random(-7,7), PV_Random(-7,7), 10, firetrailparticles, 3);
}

func ExplosionEffect(...)
{
	return _inherited(...);
}

func HitObject(obj)
{
	AddFireHitEffect(obj);
	return _inherited(obj);
}

func Hit()
{
	Explode(SpellDamage);
}