// Alternative rewarding in defense goal for Knueppeln.

#appendto Goal_Defense

public func OnClonkDeath(object clonk, int killed_by)
{
	if (clonk.Bounty)
	{
		if (killed_by != NO_OWNER)
		{
			DoMaxMagic(killed_by, clonk.Bounty);
		}
		else
		{		
			DoSharedMaxMagic(clonk.Bounty);
		}
	}
	return _inherited(clonk, killed_by, ...);
}

local shared_magic_remainder = 0;

public func DoMaxMagic(int plr, int amount)
{
	var crew = GetCrew(plr);
	if (crew)
		crew.MaxMagic += 150 * amount;
	return;
}

public func DoSharedMaxMagic(int amount)
{
	// Split gold among all players. Keep track of remainder and use it next time.
	shared_magic_remainder += amount;
	var cnt = GetPlayerCount(C4PT_User);
	if (cnt)
	{
		var magic_add = shared_magic_remainder / cnt;
		if (magic_add)
		{
			shared_magic_remainder -= magic_add * cnt;
			DoMaxMagicForAll(magic_add);
		}
	}
	return true;
}

// Add wealth to all players.
public func DoMaxMagicForAll(int amount)
{
	for (var plr in GetPlayers(C4PT_User))
		DoMaxMagic(plr, amount);
	return;
}

// Only one of each type of champ for the defenders.
public func UseTeamExclusiveChampions() { return true; }
