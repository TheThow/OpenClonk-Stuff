#include Projectile

local pR = 50;
local pG = 255;
local pB = 50;
local Speed = 90;
local SpellDamage = 25;
local Size = 20;
local ManaCost = 20;

local counter = 0;

func Initialize()
{
	SetRDir(10);
}

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
		Size = PV_Linear(PV_Random(7, 12),0),
		BlitMode = GFX_BLIT_Additive,
		R = pR,
		G = pG,
		B = pB,
	};
	
	CreateParticle("Flash", PV_Random(-3,3), PV_Random(-3,3), PV_Random(-7,7), PV_Random(-7,7), 10, trailparticles, 7);
}

func HitEffect()
{
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
	if(counter < 1)
	{
		Bounce();
		Sound("Hits::Materials::Rock::RockHit?");
		counter++;
		return;
	}

	HitEffect();
	
	for(var o in FindObjects(Find_Distance(SpellDamage), Find_Func("CanBeHit")))
	{
		AddEarthHitEffect(o);
	}
	Explode(SpellDamage);
}

func Bounce()
{
	var angle = Angle(0, 0, GetXDir(), GetYDir());
	
	var surface_angle = Angle(0, 0, GetX(), GetY());
	var angle_diff = GetTurnDirection(angle, surface_angle);
	var new_angle = surface_angle + angle_diff;
	
	SetXDir(Sin(new_angle, GetXDir()));
	SetYDir(-Cos(new_angle, GetYDir()));
}