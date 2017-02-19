// Use in scenario scripts to init statistics.

global func InitStatistics()
{
	CreateObject(Statistics_ChampSelection);
	return _inherited(...);
}