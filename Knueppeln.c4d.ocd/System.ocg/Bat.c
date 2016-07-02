#appendto Bat

func CanBeHit() { return true; }

func WillAttackPrey()
{
	return true;
}

// Lets the bat bite a pray animal, assumes the bat is close to the prey.
private func BitePrey(object prey)
{
	// A small blow to the prey and heal the bat itself.
	prey->DoEnergy(-10);
	prey->~CatchBlow(10, this);
	DoEnergy(10);
	// Make a bite/attack sound.
	Sound("Hits::ProjectileHitLiving*");
	return;
}