
#appendto DefenseTower


public func Initialize()
{
	var firing_rate = 50;
	CreateEffect(FxFiringControl, 1, firing_rate, TowerProjectile);
	return;
}

public func Death()
{
	AnnounceDeath(GetOwner());
	Fireworks();
	Explode(20);
	return;
}

public func AnnounceDeath(int owner)
{
	for (var plr in GetPlayers(C4PT_User))
	{
		var crew = GetCrew(plr);
		if (!crew)
			continue;
		if (Hostile(owner, plr))
			crew->Sound("$SndEnemyTowerDestroyed$", true, 100, plr);
		else if (GetCrew(plr))
			crew->Sound("$SndYourTowerDestroyed$", true, 100, plr);
	}
}

public func CanBeHit(object by) { return true; }