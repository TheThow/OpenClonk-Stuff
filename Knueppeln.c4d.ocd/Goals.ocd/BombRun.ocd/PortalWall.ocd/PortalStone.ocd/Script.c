/**
	Flesh
	

	@author 
*/


local Master;
local MaxEnergy = 100000;

local sharpflame;
local lightning;

func IsProjectileTarget(object o) { return true; }

func CannotBeSucked() { return true; }

func Initialize()
{
	SetAction("Travel");
	AddEffect("Particles", this, 20, 2, this);
	
	var smoke = 
	{
		ForceX = PV_Wind(200 - 180),
		DampingX = 900, DampingY = 900,
		Alpha = PV_Linear(255, 0),
		R = 100, G = 100, B = 100,
		Size = PV_Linear(PV_Random(4, 10), PV_Random(20, 30)),
		Phase = PV_Random(0, 15)
	};
	CreateParticle("Smoke", PV_Random(-5,5), PV_Random(-5,5), PV_Random(-15, 15), PV_Random(-15, 15), 30, smoke, 15);
	
	
	sharpflame =
	{
		Size = 10,
		R = 50,
		G = 255,
		B = 50,
		Alpha = 20,
		Rotation = PV_Random(0, 360),
		Phase = PV_Random(0, 5),
		OnCollision = 0,
		BlitMode = GFX_BLIT_Additive,
	};
	
		
	lightning =
	{
		Prototype = Particles_ElectroSpark2(),
		Size = PV_Linear(PV_Random(2,5),0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Random(0,360),
		R = 50,
		G = 255,
		B = 50,
	};
}

func SetMaster(master)
{
	Master = master;
}


func FxParticlesTimer(object target, proplist effect, int time)
{
	//CreateParticle("FireSharp", RandomX(-5, 5), RandomX(-5, 5), RandomX(-1, 1), PV_Random(-1,-2), 20, sharpflame, 1);
	//CreateParticle("Lightning", RandomX(-5, 5), RandomX(-5, 5), 0, 0, 10, lightning, 2);
}

func FxParticlesStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;

	SetSolidMask();
	
	var props = 
	{
		Size = PV_Linear(4, 0),
	    ForceY = GetGravity(),
		Stretch = PV_Speed(1000, 500),
		Rotation = PV_Direction(),
		OnCollision = PC_Die(),
		CollisionVertex = 0,
	    R = 100,
	    G = 100,
	    B = 100,
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("Smoke", 0, 0, PV_Random(-10 , 10), PV_Random(-10, 20), 50, props, 10);
	
	var smoke = 
	{
		ForceX = PV_Wind(200 - 180),
		DampingX = 900, DampingY = 900,
		Alpha = PV_Linear(255, 0),
		R = 100, G = 100, B = 100,
		Size = PV_Linear(PV_Random(4, 10), PV_Random(20, 30)),
		Phase = PV_Random(0, 15)
	};
	CreateParticle("Smoke", PV_Random(-5,5), PV_Random(-5,5), PV_Random(-15, 15), PV_Random(-15, 15), 30, smoke, 15);
}

func FxParticlesDamage(object target, proplist effect, int damage, int cause, int by)
{
	if(Master)
		Master->GotDamage(damage, by);
	
	return 0;
}

func CanBeHit(object from)
{
	return true;
}

func IsWallElement() { return true; }

/*
func Damage(int change, int cause, int by_player)
{
	if(Master)
		Master->GotDamage(change, by_player);
		
	DoEnergy(change);
}*/

local ActMap = {

	Travel = {
		Prototype = Action,
		Name = "Travel",
		Procedure = DFA_FLOAT,
		NextAction = "Travel",
		Length = 1,
		Delay = 1,
		FacetBase = 1,
	},
};

