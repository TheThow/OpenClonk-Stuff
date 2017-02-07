// Destroy targets when they die.

#appendto Idol
#appendto InventorsLab

public func Death()
{
	Explode(20);
	return;
}