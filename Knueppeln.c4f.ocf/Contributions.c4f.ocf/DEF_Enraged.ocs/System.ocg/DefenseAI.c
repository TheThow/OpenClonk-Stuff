// Make Knueppeln AI work for defense rounds.

#appendto KnueppelnAI

// Alternative target finding for defense scenarios: partially controlled by the scenario script.
public func FindTarget(effect fx)
{
	var target = _inherited(fx, ...);
	// Focus on defense target if normal target is too far away.
	if (!target || ObjectDistance(target, fx.Target) > fx.control.AltTargetDistance)
	{
		if (fx.is_siege)
			target = GetRandomSiegeTarget(fx.Target);
		else		
			target = GetRandomAttackTarget(fx.Target);
	}
	// If target can't be attacked just take normal target again.
	if (!this->HasWeaponForTarget(fx, target))
		target = _inherited(fx, ...);
	return target;
}