// Use in scenario scripts to init statistics.

static const STATS_Type_None = 0,
             STATS_Type_ChampSelection = 1 << 1,
             STATS_Type_ChampKillsDeaths = 1 << 2,
             STATS_Type_ChampSpellUsage = 1 << 3,
             STATS_Type_All = ~0;

global func InitStatistics(int type)
{
	if (type & STATS_Type_ChampSelection)
		CreateObject(Statistics_ChampSelection);
	if (type & STATS_Type_ChampKillsDeaths)
		CreateObject(Statistics_ChampKillsDeaths);
	if (type & STATS_Type_ChampSpellUsage)
		CreateObject(Statistics_ChampSpellUsage);
	return _inherited(...);
}