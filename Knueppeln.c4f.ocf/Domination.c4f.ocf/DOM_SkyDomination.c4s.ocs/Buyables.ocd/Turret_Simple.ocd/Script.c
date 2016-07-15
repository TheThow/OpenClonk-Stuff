/**
	Turret_Simple
	Simple turret.

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local IsTurret = true;
local IsLocked = false;

local owner_particles;
local particles;
local shine_particles;

local HitPoints = 100;
public func IsProjectileTarget() { return true; }
public func CanBeHit() { return true; }

local ColorR = 255;
local ColorG = 255;
local ColorB = 50;

func Initialize()
{
}

public func IsKnueppelItem() { return true; }

public func RejectUse(object clonk)
{
	return !clonk->IsWalking() || FindObject(Find_Distance(50), Find_Property("IsTurret"), Find_Property("IsLocked"));
}

public func ControlUse(object clonk)
{
	// Knueppeln hack
	if (RejectUse(clonk))
	{
		clonk->Sound("UI::Error", nil, nil, clonk->GetOwner());
		return true;
	}
	SetOwner(clonk->GetOwner());
	clonk->DoKneel();
	Lock();
	return true;
}

public func Destruction()
{
	particles = 
	{
		Size = PV_KeyFrames(0, 0, 0, 100, 10, 1000, 0),
		Alpha = 30,
		DampingX = 900, DampingY = 900,
		BlitMode = GFX_BLIT_Additive,
		R = ColorR, G = ColorG, B = ColorB,
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		Stretch = PV_Speed(5000),
		Rotation = PV_Direction()
	};
	CreateParticle("SphereSpark", 0, 0, PV_Random(-50, 50), PV_Random(-50, 50), 5, particles, 100);
	SoundAt("Fire::BlastMetal", 0, 0, {volume = 50, pitch = 50});
}

private func InitOwnerParticles()
{
	owner_particles = 
	{
		Size = PV_Sin(PV_Step(5), 5, 15),
		Attach = ATTACH_Front | ATTACH_MoveRelative
	};
	owner_particles = Particles_Colored(owner_particles, GetPlayerColor(GetOwner()));
	CreateParticle("SphereSpark", 0, 0, 0, 0, 0, owner_particles, 1);
}

private func InitParticles()
{
	particles = 
	{
		Size = PV_KeyFrames(0, 0, 0, 100, 3, 1000, 0),
		ForceX = PV_Sin(PV_Step(PV_Step(1, 0, 7, 20), PV_Random(0, 360, nil, 1)), 10),
		ForceY = PV_Cos(PV_Step(PV_Step(1, 0, 7, 20), PV_Random(0, 360, nil, 1)), -10),
		DampingX = 900, DampingY = 900,
		//BlitMode = GFX_BLIT_Additive,
		R = ColorR, G = ColorG, B = ColorB,
		CollisionVertex = 0,
		OnCollision = PC_Die()
	};
	
	shine_particles = 
	{
		Size = PV_KeyFrames(0, 0, 60, 500, PV_Random(50, 70), 1000, 60),
		Alpha = PV_KeyFrames(0, 0, 0, 100, 30, 1000, 0),
		BlitMode = GFX_BLIT_Additive,
		R = ColorR, G = ColorG, B = ColorB,
		Rotation = PV_Step(20)
	};
}

local DamageWatchEffect = new Effect
{
	Damage = func(int amount)
	{
		this.Target.HitPoints -= amount;
		if (this.Target.HitPoints <= 0)
			this.Target->RemoveObject();
	}
};

private func Lock()
{
	IsLocked = true;
	Exit();
	Sound("Structures::FinishBuilding", {volume = 50, pitch = 200});
	Sound("Fire::Spark1", {volume = 50, pitch = 1});
	InitOwnerParticles();
	this->InitParticles();
	SetCategory(C4D_StaticBack);
	AddTimer("DoParticles", 2);
	AddTimer("DoShineParticles", 10);
	CreateEffect(DamageWatchEffect, 1, 0);
}

public func DoParticles()
{
	CreateParticle("StarSpark", 0, 0, 0, 0, PV_Random(40, 200), particles, 10);
}

public func DoShineParticles()
{
	CreateParticle("Shockwave", 0, 0, 0, 0, 30, shine_particles, 1);
}