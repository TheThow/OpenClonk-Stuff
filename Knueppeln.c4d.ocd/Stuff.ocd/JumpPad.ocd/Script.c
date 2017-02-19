/**
	JumpPad
	@author 
*/


public func Initialize()
{
	CreateEffect(FxJumpPad, 1, 1);
	return;
}

local FxJumpPad = new Effect
{
	Timer = func()
	{
		var r = Target->GetR() - 90;
		for (var obj in Target->FindObjects(Find_OnLine(Cos(r - 75, 18), Sin(r - 75, 22), Cos(r + 75, 18), Sin(r + 75, 22)), Find_ID(Clonk)))
		{
			if (GetEffect("JumpPadCD", obj))
				continue;
			obj->SetAction("Jump");
			obj->SetVelocity(Target->GetR(), Target.Strength);
			obj->AddEffect("JumpPadCD", obj, 1, 5);
			Target->Sound("boing", false, 50);
		}
		return FX_OK;
	}
};

public func SaveScenarioObject(proplist props)
{
	if (!inherited(props, ...))
		return false;
	if (Strength != JumpPad.Strength)
		props->AddSet("Strength", this, "Strength", this.Strength);
	return true;
}


/*-- Properties --*/

local Name = "$Name$";
local Description = "$Description$";
local Strength = 100;