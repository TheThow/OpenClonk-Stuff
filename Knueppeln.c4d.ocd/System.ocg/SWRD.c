#appendto Sword

func ControlThrow()
{
	return true;
}

func SwordDamage(int shield)
{
	return ((100-shield)*25*1000 / 100);
}

func ApplyWeaponBash(pTo, int strength, angle, object from)
{
	return 0;
}

func WeaponDamage(object target, int damage, int damage_type, bool exact_damage)
{
	var angle = Angle(Contained()->GetX(), Contained()->GetY(), target->GetX(), target->GetY()) + 25;
	target->Fling(Sin(angle, 55), -Abs(Cos(angle, 55)), 10);
	return _inherited(target, damage, damage_type, exact_damage);
}

public func ControlUse(object clonk, int x, int y)
{
	// Callback for statistics collection.
	GameCallEx("OnChampUseSword", clonk);
	return _inherited(clonk, x, y, ...);
}