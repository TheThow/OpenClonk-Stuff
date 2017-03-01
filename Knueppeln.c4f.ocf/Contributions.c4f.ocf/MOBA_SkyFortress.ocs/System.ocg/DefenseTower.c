
#appendto DefenseTower

local ParentTower;

public func Initialize()
{
	var firing_rate = 50;
	CreateEffect(FxFiringControl, 1, firing_rate, TowerProjectile);
	CreateEffect(FxNoDMGOnParentExist, 1, 1);
	return;
}

public func SetParentTower(object tower)
{
	ParentTower = tower;
}

public func GetParentTower()
{
	return ParentTower;
}

local FxNoDMGOnParentExist = new Effect
{
	Damage = func(dmg, cause, plr)
	{
		if (cause == 32)
			return dmg;
		if (Target->GetParentTower())
			return 0;
		
		return dmg;
	}
};

public func Death()
{
	AnnounceDeath(GetOwner());
	Fireworks();
	Explode(20);
	return;
}

public func AnnounceDeath(owner)
{
	for(var i = 0; i < GetPlayerCount(); i++)
	{
		var plr = GetPlayerByIndex(i);
		if (GetPlayerType(plr) == C4PT_Script) continue; // Minion Players have no crew and need no announcements
		if (Hostile(owner, plr))
			GetCrew(plr)->Sound("$SndEnemyTowerDestroyed$", true, 100, plr);
		else
			GetCrew(plr)->Sound("$SndYourTowerDestroyed$", true, 100, plr);
	}
}

public func CanBeHit(object by) { return true; }