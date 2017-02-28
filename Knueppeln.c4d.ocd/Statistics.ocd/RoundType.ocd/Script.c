/**
	Statistics: Round Type
	Collects round type statistics, types are:
	 * Deathmatch
	 * Defense
	 * BombRun
	 * Domination
	 * Moba
	
	@author Maikel
*/


local stats;

public func Initialize()
{
	// Register round type if goal already exists.
	if (FindObject(Find_ID(Goal_LastManStanding)))
		RegisterRoundType("Deathmatch");
	else if (FindObject(Find_ID(Goal_Defense)))
		RegisterRoundType("Defense");
	else if (FindObject(Find_ID(Goal_BombRun)))
		RegisterRoundType("BombRun");
	else if (FindObject(Find_ID(Goal_Domination)))
		RegisterRoundType("Domination");
	else if (FindObject(Find_ID(GetDefinition("Goal_Moba"))))
		RegisterRoundType("Moba");
	return;
}

public func RegisterRoundType(string round_type)
{
	stats = round_type;
	return;
}

public func CollectStats()
{
	var round_type = FindObject(Find_ID(Statistics_RoundType));
	if (round_type)
		return round_type->GetStats();
	return nil;
}

public func GetStats() { return stats; }