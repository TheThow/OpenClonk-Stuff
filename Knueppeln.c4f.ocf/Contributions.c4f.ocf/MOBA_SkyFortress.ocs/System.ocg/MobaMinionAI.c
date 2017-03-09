// Adapt minion AI to this scenario.

#appendto MobaMinionAI

public func Execute(effect fx, int time)
{
	// Don't do anything when still in respawn container.
	if (fx.Target->Contained())
		return;
		
	if (fx.Target.Lane == 1)
	{
		// Bot needs to drop through a bridge
		if (fx.Target.Team == 1)
		{
			if (fx.Target->GetX() > 500 && fx.Target->GetX() < 570)
			{
				fx.Target->HalfVehicleFadeJumpStart();
			}
			if (fx.Target->GetX() > 570)
			{
				fx.Target->HalfVehicleFadeJumpStop();
			}
		}
		
		if (fx.Target.Team == 2)
		{
			if (fx.Target->GetX() < 5900 && fx.Target->GetX() > 5830)
			{
				fx.Target->HalfVehicleFadeJumpStart();
			}
			if (fx.Target->GetX() < 5830)
			{
				fx.Target->HalfVehicleFadeJumpStop();
			}
		}
	}
	
	// Check for near targets
	var near = fx.Target->FindObject(Find_OCF(OCF_Alive), Find_Distance(100), Find_Hostile(fx.Target->GetOwner()), Sort_Distance());
	if (near)
		fx.target = near;
			
	// Do normal AI behavior.
	return inherited(fx, time, ...);
}