#include Projectile


func InitEffect()
{
	Sound("electric_shot", false, 100);
	Sound("electro_travel", false, 50, nil, 1);
	SetLightRange(30, 70);
	SetLightColor(RGB(255, 255, 255));

	var lightparticle =
	{
		Alpha = 30,
		Size = 30,
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Step(30, 0, 1),
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	CreateParticle("Flash", 0, 0, 0, 0, 0, lightparticle);
	
	var lightparticle2 =
	{
		Alpha = 255,
		Size = 8,
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Step(30, 0, 1),
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	CreateParticle("Shockwave", 0, 0, 0, 0, 0, lightparticle2, 2);
}

func TravelEffect(int time)
{
	var trailparticles =
	{
		Prototype = Particles_ElectroSpark2(),
		Size = PV_Linear(RandomX(10,20),0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = Angle(0,0,GetXDir(), GetYDir()),
		ForceX = PV_Random(-7,7),
		ForceY = PV_Random(-7,7),
	};
	
	CreateParticle("ElectroSpark", 0, 0, 0, 0, 10, trailparticles, 5);
}

func ExplosionEffect(int level, int x, int y, int smoothness, bool silent, int damage_level)
{
	var particles =
	{
		Prototype = Particles_Glimmer(),
		R = 150,
		G = 200,
		B = 255,
		Alpha = 255,
		Size = PV_Linear(10, 0),
	};
	CreateParticle("StarSpark", x, y, PV_Random(-50,50), PV_Random(-50, 50), 20, particles, 25);
	
	var sphereparticle =
	{
		Alpha = PV_Linear(255, 0),
		Size = level,
		R = 150,
		G = 200,
		B = 255,
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("StarSpark", x, y, 0, 0, 10, sphereparticle, 4);
	Sound("electro_explosion", false, 50);
	
}