/**
	Minion AI
	Just minions.

	@author K-Pone
*/


#include AI


public func Execute(effect fx, int time)
{
	// Don't do anything when still in respawn container.
	if (fx.Target->Contained())
		return;
	
	//Log("%v %v", fx.Target.Lane, fx.target);
	
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

public func ExecuteEvade(effect fx, int threat_dx, int threat_dy)
{
	return false;
}

public func ExecuteProtection(effect fx)
{
	return false;
}

public func FindTarget(effect fx)
{
	var target = _inherited(fx, ...);
	if (!target || ObjectDistance(target, fx.Target) > 150)
		target = GetRandomAttackTarget(fx.Target);
	return target;
}


/*-- AI Overloading --*/

public func ExecuteIdle(effect fx)
{
	// Just stay at current position and wait for enemies.
	return true;
}

// Checks whether the AI has a weapon for the target.
public func HasWeaponForTarget(effect fx, object target)
{
	if (target && target->~CanBeHit())
		return true;
	return _inherited(fx, target, ...);
}


/*-- Aiming Cursor --*/

// Called by spell charging effects.
public func GetAICursor(fx)
{
	if (fx.target)
		return [fx.target->GetX() - fx.Target->GetX(), fx.target->GetY() - fx.Target->GetY()];
	 // If the AI has no target, aim above its head.	
	return [0, 5];
}
