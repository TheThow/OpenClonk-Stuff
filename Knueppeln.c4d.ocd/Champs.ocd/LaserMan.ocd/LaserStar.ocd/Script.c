/**
	ElectroOrb
	

	@author 
*/

local ManaCost = 10;

local SpellDamage = 10;
local Speed = 25;
local Durr = 35;

local Size = 15;

local Name = "$Name$";
local Description = "$Description$";

local ChargeTime = 38;
local Length = 150;
local TargetAngle = 0;
local shooter;

func Initialize()
{
	SetCategory(C4D_StaticBack);
}

func Launch(object clonk, int x, int y)
{
	SetController(clonk->GetOwner());
	TargetAngle = Angle(0, 0, x, y);
	shooter = clonk;
	AddEffect("Lasers", this, 1, 2, this, nil);
	SoundAt("LaserMan::LightStar", nil, nil, nil, nil, nil, Random(30));
	
	
	this.particle_stars = 
	{
		Stretch = PV_Random(15000, 20000),
		Size = PV_Linear(2, 0),
		Alpha = PV_Random(255,0,3),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Random(0, 360),
		G = 0, B = 0
	};
	
	this.particle_ray = 
	{
		Size = 16,
		Alpha = PV_Linear(255,0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = TargetAngle,
		G = 0, B = 0
	};
}

func Remove()
{
	RemoveObject();
}

public func HitObject(obj)
{
	if(obj->~CanBeHit(this) == false)
		return;
		
	obj->AddLaserHitEffect();
	obj->SetSpeed(7 * obj->GetXDir(1) / 10, 7 * obj->GetYDir(1) / 10, 1); 
	WeaponDamage(obj, SpellDamage);
}

func FxLasersTimer(target, fx, time)
{
	if (time < ChargeTime - 1)
	{
		CreateParticle("StarSpark", 0, 0, 0, 0, 10, this.particle_stars, 2);
		return FX_OK;
	}
	this->Call(LaserRay.DoTheLaser, 0, 0, Sin(TargetAngle, Length), -Cos(TargetAngle, Length), TargetAngle, Length, true, true);
	RemoveObject();
}
