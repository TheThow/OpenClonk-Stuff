local ManaCost = 10;
local Duration = 90;

public func Launch(object clonk, int x, int y)
{
	var particles =
	{
		Size = 1, 
		R = 50,G = 255,B = 50,Alpha = PV_Linear(255,0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Direction(), 
		Stretch = PV_Linear(PV_Speed(1000*5),1000),
		ForceY = PV_Gravity(500),
		CollisionVertex = 500,
		OnCollision = PC_Bounce(250)
	};

	CreateParticle("Magic",0, 0, PV_Random(-50,50), PV_Random(-50,50), 108,  particles, 30);

	clonk -> CreateEffect(FxMountainBrew,100,2,this.Duration);

	return RemoveObject();
}

local FxMountainBrew = new Effect 
{
	Construction = func(int duration)
	{
		this.Target -> SetClrModulation(RGBa(0, 255, 0, 255));
		this.duration= duration;
		return FX_OK;
	},
	Timer = func(int time)
	{
		this.Target -> DoMagicEnergy(1000, true, 1);

		var particles =
		{
			Size = PV_Linear(4,0), 
			R = 50,G = 255,B = 50,Alpha = PV_Linear(255,0),
			BlitMode=GFX_BLIT_Additive,
		};
		this.Target -> CreateParticle("MagicRing",PV_Random(-3,3),PV_Random(-13,-7), PV_Random(-10,10), PV_Random(-10,-15), 12,  particles, 1);

		if (time >=  this.duration)
			return FX_Execute_Kill;
		return FX_OK;
	},
	Destruction = func()
	{
		if (!this.Target)
			return FX_OK;
		this.Target -> SetClrModulation(RGBa(255, 255, 255, 255));
		return FX_OK;
	}
};
