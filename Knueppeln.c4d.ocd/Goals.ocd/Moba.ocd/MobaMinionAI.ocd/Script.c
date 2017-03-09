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

	// Check for near targets
	var near = fx.Target->FindObject(Find_OCF(OCF_Alive), Find_Distance(100), Find_Hostile(fx.Target->GetOwner()), fx.Target->Find_PathFree(), Sort_Distance());
	if (near)
		fx.target = near;
			
	// Do normal AI behavior.
	return inherited(fx, time, ...);
}

// Don't evade anything.
public func ExecuteEvade(effect fx, int threat_dx, int threat_dy)
{
	return false;
}

// Don't protect against anything.
public func ExecuteProtection(effect fx)
{
	return false;
}

// Get normal target if close and path is free, otherwise get target from scenario callback.
public func FindTarget(effect fx)
{
	var target = _inherited(fx, ...);
	if (!target || ObjectDistance(target, fx.Target) > 150 || !PathFree(target->GetX(), target->GetY(), fx.Target->GetX(), fx.Target->GetY()))
		target = GetRandomAttackTarget(fx.Target);
	return target;
}

// Don't get any special emergency targets.
public func FindEmergencyTarget(effect fx)
{
	return this->FindTarget(fx);
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
