#include Projectile

local pR = 225;
local pG = 215;
local pB = 30;
local Speed = 82;
local SpellDamage = 65;
local Size = 20;
local ManaCost = 60;

local ChargeDuration = 15;

func Initialize()
{
	SetAction("Travel");
	//SetClrModulation(RGBa(0,0,0,0));
}

func InitEffect()
{
	Sound("Objects::Boompack::Launch", false, 100, nil, 0, nil, -60);
	Sound("Objects::Boompack::Fly",    false, 50, nil, 1, nil, -50);
	SetLightRange(30, 70);
	SetLightColor(RGB(200, 215, 255));
	
	SetGraphics(nil, DefenseBoomAttack);
}

func Launch(object clonk, int x, int y)
{
	var particlescharge =
	{
		R = PV_Random(200,255),
		G = PV_Random(200,255),
		B = PV_Random(200,255),
		Alpha = PV_Linear(0, 150),
		Size = PV_Linear(PV_Random(10,15),0),
		Rotation = PV_Linear(PV_Random(0,360),PV_Random(0,360)),
	};
	
	for(var i = 0; i < 360; i += 3)
	{
		clonk->CreateParticle("BlueSpark", Sin(i, 15), -Cos(i, 15), Sin(i+180, 20), -Cos(i+180, 20), 40, particlescharge);
	}
	
	
	clonk->Charge(this, "ChargeStop", ChargeDuration, {clonk = clonk, x = x, y = y});
}

func ChargeStop(params)
{
	RealLaunch(params.clonk, params.x, params.y);
}

func ChargeInterrupted()
{
	RemoveObject();
}

func RealLaunch(object clonk, int x, int y)
{
	var angle = Angle(0,0,x,y, 10);
	shooter = clonk;
	SetVelocity(angle, Speed, 10);
	SetController(clonk->GetController());
	AddEffect("HitCheck", this, 1,1, nil,nil, clonk);
	AddEffect("TheEffect", this, 1, 1, this, Projectile);
}

func TravelEffect(int time)
{
	var trailparticles =
	{
		Prototype = Particles_ElectroSpark2(),
		Size = PV_Linear(PV_Random(5,15),0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = Angle(0,0,GetXDir(), GetYDir()),
		R = 250,
		G = 100,
		B = 30,
		Alpha = 50,
	};
	
	var angle = Angle(0,0, GetXDir(), GetYDir()) + 180;
	
	SetR(angle+180);
	this["MeshTransformation"] = Trans_Rotate(time*10, 0, 1, 0);
	
	CreateParticle("FireDense", Sin(angle, 7), -Cos(angle, 7) - 5, PV_Random(-10,10), PV_Random(-10,10), 10, Particles_Thrust(), 5);
	
	if (time > 1000) Explode(SpellDamage);
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
	
	Sound("Fire::BlastMetal");
	Sound("Hits::Materials::Glass::GlassShatter");
	
	Fireworks();
	
}

func Hit()
{
	HitEffect();
	
	for(var o in FindObjects(Find_Distance(Size), Find_Func("CanBeHit", this)))
	{
		var angle = Angle(GetX(), GetY(), o->GetX(), o->GetY());
		o->SetVelocity(angle, 10);
		WeaponDamage(o, SpellDamage);
	}
	
	RemoveObject();
}