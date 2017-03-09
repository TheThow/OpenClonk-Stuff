// Adapt minion AI to this scenario.

#appendto MobaMinionAI

public func Execute(effect fx, int time)
{
	// Don't do anything when still in respawn container.
	if (fx.Target->Contained())
		return;
		
	// Ensure the minions do not walk of the cliffs on the right and left sides.
	if (!Inside(fx.Target->GetX(), 96, LandscapeWidth() - 96))
	{
		fx.Target->SetCommand("None");
		if (fx.Target->GetX() < 96)
			fx.Target->SetComDir(COMD_Right);
		else if (fx.Target->GetX() > LandscapeWidth() - 96)
			fx.Target->SetComDir(COMD_Left);	
		return;
	}
	
	// Move into lanes.
	// Upper.
	if (fx.Target.Lane == 0)
	{
		
	}
	// Middle.
	else if (fx.Target.Lane == 1)
	{
		if ((fx.Target.Team == 1 && Inside(fx.Target->GetX(), 220, 260)) || (fx.Target.Team == 2 && Inside(fx.Target->GetX(), 1664 - 260, 1664 - 220)))
			return this->ExecuteJump(fx);
	}
	// Lower.
	else if (fx.Target.Lane == 2)
	{
		if ((fx.Target.Team == 1 && Inside(fx.Target->GetX(), 220, 260)) || (fx.Target.Team == 2 && Inside(fx.Target->GetX(), 1664 - 260, 1664 - 220)))
			return this->ExecuteJump(fx);
		if ((fx.Target.Team == 1 && Inside(fx.Target->GetX(), 365, 410)) || (fx.Target.Team == 2 && Inside(fx.Target->GetX(), 1664 - 410, 1664 - 365)))
		{
			fx.Target->HalfVehicleFadeJumpStart();
			ScheduleCall(fx.Target, "HalfVehicleFadeJumpStop", 36, 0);
			return;
		}
	}
	
	// Check for near targets
	var near = fx.Target->FindObject(Find_OCF(OCF_Alive), Find_Distance(100), Find_Hostile(fx.Target->GetOwner()), Sort_Distance());
	if (near)
		fx.target = near;
			
	// Do normal AI behavior.
	return inherited(fx, time, ...);
}