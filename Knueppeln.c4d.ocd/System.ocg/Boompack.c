#appendto Boompack

public func IsCarryHeavy() { return false; }
public func CanBeHit(object o) { return true; }

func Initialize()
{
	AddEffect("CheckDmg", this, 1, 100, this);
	return _inherited();
}

func FxCheckDmgTimer() { return 0; }
func FxCheckDmgDamage() { Fuse(); }
func IsProjectileTarget(object o) { return o->GetID() != Sword; }