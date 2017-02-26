#include SprayAndPray

local Speed = 500;
local SpellDamage = 15;
local ManaCost = 40;

local ChargeDuration = 15;

public func InitEffect()
{
	Sound("Objects::Weapons::Blunderbuss::GunShoot?", false, 50);
	SetLightRange(30, 70);
	SetLightColor(RGB(255, 255, 255));
	ox = GetX();
	oy = GetY();
	
	trailparticles =
	{
		Size = PV_Linear(PV_Random(2, 3),0),
		BlitMode = GFX_BLIT_Additive,
		R = 255,
		G = 100,
		B = 100,
	};
	
}

public func Launch(object clonk, int x, int y)
{
	var particlescharge =
	{
		R = 255,
		G = 0,
		B = 0,
		Alpha = PV_Linear(0, 150),
		Size = PV_Linear(10,0),
	};
	
	for(var i = 0; i < 360; i += 3)
	{
		if (i < 120) {particlescharge.R -= 6; particlescharge.G += 6;}
		if (i >= 120 && i < 240) {particlescharge.G -= 6; particlescharge.B += 6;}
		if (i >= 240) {particlescharge.B -= 6; particlescharge.R += 6;}
		clonk->CreateParticle("Magic", Sin(i, 20), -Cos(i, 20), Sin(i+180, 10), -Cos(i+180, 10), 20, particlescharge);
	}

	clonk->SetDir(1-clonk->GetDir());
	
	clonk->Charge(this, "ChargeStop", ChargeDuration, {clonk = clonk, x = x, y = y});
}

public func ChargeStop(params)
{
	params.clonk->SetDir(1-params.clonk->GetDir());
	RealLaunch(params.clonk, params.x, params.y);
}

public func ChargeInterrupted()
{
	RemoveObject();
}

public func RealLaunch(object clonk, int x, int y)
{
	var angle = Angle(0, 0, x, y, 10);
	shooter = clonk;
	SetVelocity(angle, Speed, 10);
	SetController(clonk->GetController());
	AddEffect("HitCheck", this, 1, 1, nil, nil, clonk);
	AddEffect("TheEffect", this, 1, 1, this, Projectile);
}

public func TravelEffect(int time)
{
	trailparticles.Size=PV_Linear(4,0);
	trailparticles.R=PV_KeyFrames(1, 250, 255, 500, 0, 750, 0, 1000, 255);
	trailparticles.G=PV_KeyFrames(1, 250, 0, 500, 255, 750, 0, 1000, 0);
	trailparticles.B=PV_KeyFrames(1, 250, 0, 500, 0, 750, 255, 1000, 0);
	DrawParticleLine("Flash", 0, 0, ox-GetX(), oy-GetY(), 1, 0, 0, 12, trailparticles);
	ox=GetX();
	oy=GetY();
	
}