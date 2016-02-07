/**
	ShadowWalk
	

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local ManaCost = 30;

local Dur = 100;

func Launch(object clonk, int x, int y)
{
	CreateParticle("Smoke", PV_Random(-5,5), PV_Random(-10,10), PV_Random(-5,5), 0, 30, Particles_Smoke(), 20);
	clonk.Visibility = VIS_Owner;
	clonk->SetClrModulation(RGBa(255,255,255,50));
	Sound("vanish", false, 50);
	
	var fx = AddEffect("ShadowWalk", clonk, 20, 1, nil, GetID());
	fx.dur = Dur;
	RemoveObject();
}

func FxShadowWalkTimer(object target, proplist effect, int time)
{
	if(time >= effect.dur)
		return -1;
	
	
}

func FxShadowWalkStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary || !target)
		return;
	
	target.Visibility = VIS_All;
	target->SetClrModulation(RGBa(255, 255, 255, 255));
	Sound("reappear", false, 50);
	//target->CreateParticle("Smoke", PV_Random(-5,5), PV_Random(-10,10), PV_Random(-5,5), 0, 30, Particles_Smoke(), 20);
}

