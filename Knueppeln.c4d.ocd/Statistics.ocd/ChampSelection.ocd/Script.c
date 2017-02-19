/**
	Statistics: Champ Selection
	
	@author Maikel
*/


local stats;

public func Initialize()
{
	stats = {};
	return;
}

public func OnChampSelection(id champ, int plr)
{
	if (GetPlayerType(plr) == C4PT_Script)
		return;	
	var champ_str = Format("%i", champ);
	if (!stats[champ_str])
		stats[champ_str] = 1;
	else
		stats[champ_str]++;
	return;
}

public func CollectStats()
{
	var champ_stats = FindObject(Find_ID(Statistics_ChampSelection));
	if (champ_stats)
		return champ_stats->GetStats();
	return nil;
}

public func GetStats() { return stats; }