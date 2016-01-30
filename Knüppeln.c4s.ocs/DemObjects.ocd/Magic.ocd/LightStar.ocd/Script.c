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

local LifeTime = 65;

func Initialize()
{
	SetAction("Travel");
	SetRDir(10);
	SetClrModulation(RGBa(0,0,0,0));
}

func Launch(object clonk, int x, int y)
{
	SetController(clonk->GetOwner());
	SetVelocity(Angle(0, 0, x, y) + RandomX(-3, 3), Speed + RandomX(-5, 5));
	AddEffect("HitCheck", this, 1,1, nil,nil, clonk, true);
	ScheduleCall(this, "Hit", LifeTime);
	
	AddEffect("LightSparks", this, 1, 1, this, nil);
	SoundAt("LightMan::LightStar", nil, nil, nil, nil, nil, Random(30));
}

func Hit()
{
	RemoveObject();
}

public func HitObject(obj)
{
	obj->DoEnergy(-SpellDamage, nil, nil, GetController());
	Hit();
}

func FxLightSparksStart(target, fx, temp)
{
	if (temp) return;
	fx.particles = 
	{
		Stretch = PV_Random(6000, 10000),
		Size = PV_Linear(2, 0),
		Alpha = PV_Random(255,0,3),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Random(0, 360),
	};
}

func FxLightSparksTimer(target, fx, time)
{
	CreateParticle("StarSpark", 0, 0, 0, 0, 10, fx.particles, 2);
}

func Destruction()
{
	var particles =
	{
		Prototype = Particles_Glimmer(),
		R = 255, G = 255, B = 200,
		Stretch = PV_Speed(4000, 2000),
	};
	CreateParticle("StarSpark", 0, 0, PV_Random(-50, 50), PV_Random(-50, 50), PV_Random(5, 10), particles, 30);
}

local ActMap = {

	Travel = {
		Prototype = Action,
		Name = "Travel",
		Procedure = DFA_FLOAT,
		NextAction = "Travel",
		Length = 1,
		Delay = 1,
		FacetBase = 1,
		StartCall = "Traveling",
		Speed=1000
	},
};