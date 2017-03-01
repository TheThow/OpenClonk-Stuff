#include Projectile

local Speed = 300;
local SpellDamage = 3;
local ManaCost = 10;

local ox;
local oy;

//CoolingTime - time it takes for one charge of inaccuracy to drop off
local CoolingTime = 90;

local trailparticles;

public func Launch(object clonk, int x, int y)
{
	var angle = Angle(0, 0, x, y, 10);
	shooter = clonk;

	var inaccuracy_amount = 0;
	if (!GetEffect("FxMountainBrew",clonk)) 
		inaccuracy_amount = 15 * GetEffectCount("FxInaccuracy", clonk);
	SetVelocity(angle + RandomX(-inaccuracy_amount, inaccuracy_amount), Speed, 10);

	SetController(clonk->GetController());
	AddEffect("HitCheck", this, 1, 1, nil, nil, clonk);
	AddEffect("TheEffect", this, 1, 1, this, Projectile);
	clonk->CreateEffect(FxInaccuracy, 100, 1, this.CoolingTime);
	return;
}

local FxInaccuracy = new Effect 
{
	Construction = func(int CoolingTime)
	{
		this.CoolingTime = CoolingTime;
		return FX_OK;
	},
	Timer = func(int time)
	{
		var inaccuracy_amount = GetEffectCount("FxInaccuracy", this.Target);

		var particles =
		{
			Size = PV_Linear(2,0), 
			R = 28 * inaccuracy_amount,
			G = 255 - (28 * inaccuracy_amount),
			B = 50,
			Alpha = PV_Linear(255,0),
			Attach=ATTACH_MoveRelative,
		};

		//if mountain brew effect is active, inaccuracy meter changes color to turqoise
		if (GetEffect("FxMountainBrew",this.Target))
		{
			particles.R=0;
			particles.G=255;
			particles.B=255;
		}

		var i;
		while (i < inaccuracy_amount)
		{
			this.Target->CreateParticle("Magic",5,8-i*2, PV_Random(-10,10), PV_Random(-inaccuracy_amount,inaccuracy_amount), 6,  particles, 1);
			i++;
		}

		if (time >= this.CoolingTime)
			return FX_Execute_Kill;
		return FX_OK;
	}

};

public func Initialize()
{
	SetAction("Travel");
}

public func Construction()
{
	this.ActMap = new Projectile.ActMap { Travel = new Projectile.ActMap.Travel { Speed = 10000 }};
}

public func InitEffect()
{
	Sound("Objects::Weapons::Blunderbuss::GunShoot?", false, 50);
	SetLightRange(30, 70);
	SetLightColor(RGB(255, 100, 100));
	ox=GetX();
	oy=GetY();
	
	trailparticles =
	{
		Size = PV_Linear(PV_Random(2, 3),0),
		BlitMode = GFX_BLIT_Additive,
		R = 255,
		G = 100,
		B = 100,
	};
	
}

public func TravelEffect(int time)
{

	DrawParticleLine("Flash", 0, 0, ox-GetX(), oy-GetY(), 1, 0, 0, 8, trailparticles);
	ox=GetX();
	oy=GetY();
	
}

public func HitObject(object obj)
{
	if (!obj->~CanBeHit())
		return;
		
	var sphereparticle =
	{
		Alpha = 255,
		Size = 10,
		R = 255,
		G = 255,
		B = 255,
	};

	var wowgfx =
	{
		Alpha = 255,
		Size = 10,
		R = PV_KeyFrames(1, 250, 255, 500, 0, 750, 0, 1000, 255),
		G = PV_KeyFrames(1, 250, 0, 500, 255, 750, 0, 1000, 0),
		B = PV_KeyFrames(1, 250, 0, 500, 0, 750, 255, 1000, 0),
		Rotation = PV_Direction(),
		Phase = PV_Random(0,8),
	};

	// Headshot modifier: if hit a crew member above the head.
	if (this->GetY() <= obj->GetY() - 5 && obj->GetOCF() & OCF_CrewMember) 
	{
		sphereparticle = {Alpha = 255, Size = 10, G = 0, B = 0};
		obj->CreateParticle("Hitmarker", 0, -8, 0, 0, 7, sphereparticle, 1);
		obj->CreateParticle("Wow", 0, 0, RandomX(-10,10),  RandomX(-15,-5), 16, wowgfx, 1);

		WeaponDamage(obj, SpellDamage * 3);
		Sound("UI::Click", false, 50);
	}
	else 
	{
		obj->CreateParticle("Hitmarker", 0, 0, 0, 0, 7, sphereparticle, 1);
		WeaponDamage(obj, SpellDamage);
		Sound("UI::Click", false, 50);
	}

	RemoveObject();
}

public func Hit(int xdir, int ydir)
{
	RemoveObject();
}

