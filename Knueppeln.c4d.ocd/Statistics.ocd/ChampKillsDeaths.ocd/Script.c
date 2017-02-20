/**
	Statistics: Champ Kills & Deaths
	
	@author Maikel
*/


local stats;

public func Initialize()
{
	stats = {};
	return;
}

public func OnClonkDeath(object clonk, int killed_by)
{
	// First register kill, assume it has been made by cursor of killer.
	if (GetPlayerType(killed_by) == C4PT_User)
	{
		var killer = GetCursor(killed_by);
		if (killer && killer.ChampType != nil && killer.ChampType != Man)
		{
			var champ_str = Format("%i", killer.ChampType);
			if (!stats[champ_str])
				stats[champ_str] = [0, 0];
			stats[champ_str][0]++;
		}
	}
	// Then register death.
	if (GetPlayerType(clonk->GetOwner()) == C4PT_User)
	{
		if (clonk.ChampType != nil && clonk.ChampType != Man)
		{
			var champ_str = Format("%i", clonk.ChampType);
			if (!stats[champ_str])
				stats[champ_str] = [0, 0];
			stats[champ_str][1]++;
		}
	}
	return;
}

public func CollectStats()
{
	var champ_stats = FindObject(Find_ID(Statistics_ChampKillsDeaths));
	if (champ_stats)
		return champ_stats->GetStats();
	return nil;
}

public func GetStats() { return stats; }
