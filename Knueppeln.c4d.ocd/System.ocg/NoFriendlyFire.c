// Adapt no friendly fire rule to include spell effects.

#appendto Rule_NoFriendlyFire

public func DisableFriendlyFire(object for_obj)
{
	// Overload several functions to check for friendly fire.
	if (for_obj.CanBeHit != Rule_NoFriendlyFire.NoFF_CanBeHit)
	{
		for_obj.Backup_CanBeHit = for_obj.CanBeHit;
		for_obj.CanBeHit = Rule_NoFriendlyFire.NoFF_CanBeHit;
	}
	return _inherited(for_obj, ...);
}

public func EnableFriendlyFire(object for_obj)
{
	// Stop overloading functions to check for friendly fire.
	if (for_obj.Backup_CanBeHit != nil)
	{
		for_obj.CanBeHit = for_obj.Backup_CanBeHit;
		for_obj.Backup_CanBeHit = nil;
	}
	return _inherited(for_obj, ...);
}

public func NoFF_CanBeHit(object spell, ...)
{
	var w_controller = NO_OWNER;
	if (spell)
		w_controller = spell->GetController();
	var t_controller = this->GetController();
	if (w_controller != NO_OWNER && t_controller != NO_OWNER && !Hostile(w_controller, t_controller))
		return false;
	if (this.Backup_CanBeHit)
		return this->Backup_CanBeHit(spell, ...);
	return false;
}