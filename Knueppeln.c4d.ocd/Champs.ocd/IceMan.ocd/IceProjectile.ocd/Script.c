#include Projectile

local Speed = 40;
local MaxTime = 50;
local SpellDamage = 18;
local Radius = 30;
local trail_particles;

func CanBeSucked() { return true; }

func InitEffect()
{
	Sound("Fire::Fireball", false, 100, nil, nil, nil, 200);
	Sound("Fire::FuseLoop", false, 20, nil, 1, nil, 200);
	SetLightRange(30, 70);
	SetLightColor(RGB(100, 100, 255));

	var snow =
	{
		R = 200,
		G = 200,
		B = 255,
		Alpha = PV_Linear(255, 0),
		Size = PV_Random(0, 2),
		CollisionVertex = 0,
		OnCollision = PC_Stop(),
		ForceX = PV_Random(-5, 5, 10),
		ForceY = PV_Gravity(100),
		DampingX = 900, DampingY = 900
	};
	
	trail_particles = 
	{
		R = PV_Random(200, 255), G = PV_Random(200, 255), B = 255,
		Prototype = Particles_FireTrail(),
		Size = PV_Linear(10, 0),
		BlitMode = nil,
		Rotation = PV_Random(0, 360),
		BlitMode = GFX_BLIT_Additive
	};
	
	CreateParticle("StarFlash", 0, 0, PV_Random(-30, 30), PV_Random(-30, 30), 100, snow, 100);
}

func TravelEffect(int time)
{
	CreateParticle("StarFlash", 0, 0, PV_Random(-7,7), PV_Random(-7,7), 10, trail_particles, 3);
	if (time > MaxTime)
		Hit();
}

func ExplosionEffect(...)
{
	return _inherited(...);
}

func HitObject(obj)
{
	return _inherited(obj);
}

func Hit()
{
	Iceplosion(Radius, SpellDamage);
}

public func Iceplosion(int radius, int damage)
{
	SoundAt("Fire::Cracker", 0, 0, nil, nil, nil, 300);
	SoundAt("Fire::Fireball", 0, 0, nil, nil, nil, 300);
	
	var border = 
	{
		R = 200,
		G = 200,
		B = 255,
		Size = PV_Linear(PV_Random(2, 4), 0),
		OnCollision = PC_Stop(),
		CollisionVertex = 0,
		ForceX = PV_Linear(0, PV_Random(-10, 10, 10)),
		ForceY = PV_Linear(0, PV_Random(-10, 10, 10)),
		DampingX = 900, DampingY = 900
	};
	
	for (var angle = 0; angle < 360; angle += 20)
	{
		CreateParticle("StarFlash", Sin(angle + RandomX(-5, 5), radius), -Cos(angle + RandomX(-5, 5), radius), 0, 0, PV_Random(60, 100), border, 6);
	}
	
	var center = 
	{
		R = 200, G = 200, B = 255,
		Alpha = PV_Linear(255, 0),
		Size = PV_KeyFrames(0, 0, 0, 100, 3 * radius, 1000, 0),
		Rotation = PV_Random(0, 360),
		BlitMode = GFX_BLIT_Additive
	};
	CreateParticle("StarFlash", 0, 0, PV_Random(-2, 2), PV_Random(2, 2), 10, center, 10);
	var x = GetX();
	var y = GetY();
	var player = GetController();
	var layer = GetObjectLayer();
	RemoveObject();
	BlastObjects(x, y, radius, nil, player, damage, layer, nil);
}