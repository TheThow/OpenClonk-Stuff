/**
	Shadow Walk
	@author 
*/

local ManaCost = 30;
local Duration = 100;

public func Launch(object clonk, int x, int y)
{
	clonk->CreateEffect(FxInvisible, 100, 10, this.Duration);
	return RemoveObject();
}

local FxInvisible = new Effect
{
	Construction = func(int duration)
	{
		this.duration = duration;
		this.Target->CreateParticle("Smoke", PV_Random(-5, 5), PV_Random(-10, 10), PV_Random(-5, 5), 0, 30, Particles_Smoke(), 20);
		this.Target.Visibility = VIS_Owner;
		this.Target->SetClrModulation(RGBa(255, 255, 255, 50));
		this.Target->Sound("vanish", false, 50);
		return FX_OK;
	},
	Timer = func(int time)
	{
		if (time >= this.duration)
			return FX_Execute_Kill;
		return FX_OK;
	},
	Destruction = func()
	{
		if (!this.Target)
			return FX_OK;
		this.Target.Visibility = VIS_All;
		this.Target->SetClrModulation(RGBa(255, 255, 255, 255));
		this.Target->Sound("reappear", false, 50);
		return FX_OK;
	}
};
