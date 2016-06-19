#appendto MovingBrick

func Initialize()
{
	SetCategory(C4D_Vehicle);
	AddVertex(22, 0);
	AddVertex(-22, 0);
	AddVertex(0, 6);
	AddVertex(0, -6);
	
	var smoke = 
	{
		ForceX = PV_Wind(200 - 180),
		DampingX = 900, DampingY = 900,
		Alpha = PV_Linear(255, 0),
		R = 100, G = 100, B = 100,
		Size = PV_Linear(PV_Random(4, 10), PV_Random(20, 30)),
		Phase = PV_Random(0, 15)
	};
	CreateParticle("Smoke", PV_Random(-20,20), PV_Random(-5,5), PV_Random(-15, 15), PV_Random(-15, 15), 30, smoke, 35);
	
	return _inherited();
}

func Launch(obj, x, y)
{
	
}

func Hit()
{
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
	CreateParticle("Smoke", PV_Random(-20, 20), PV_Random(-5,5), PV_Random(-10 , 10), PV_Random(-10, 20), 50, props, 20);
	
	var smoke = 
	{
		ForceX = PV_Wind(200 - 180),
		DampingX = 900, DampingY = 900,
		Alpha = PV_Linear(255, 0),
		R = 100, G = 100, B = 100,
		Size = PV_Linear(PV_Random(4, 10), PV_Random(20, 30)),
		Phase = PV_Random(0, 15)
	};
	CreateParticle("Smoke", PV_Random(-20,20), PV_Random(-5,5), PV_Random(-15, 15), PV_Random(-15, 15), 30, smoke, 25);

	RemoveObject();
}

local ActMap = {
	Moving = {
		Prototype = Action,
		Name = "Moving",
		Procedure = DFA_FLOAT,
		Length = 1,
		Delay = 1,
		X = 0,
		Y = 0,
		Accel = 20,
		Decel = 20,
		Speed = 300,
		Wdt = 40,
		Hgt = 8,
		NextAction = "Moving",
	},
};