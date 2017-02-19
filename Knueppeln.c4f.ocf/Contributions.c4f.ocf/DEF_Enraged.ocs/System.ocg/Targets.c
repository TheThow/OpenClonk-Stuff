// Destroy targets when they die.

#appendto DefenseTower
#appendto InventorsLab

public func Death()
{
	Explode(20);
	return;
}

public func CanBeHit(object by) { return true; }