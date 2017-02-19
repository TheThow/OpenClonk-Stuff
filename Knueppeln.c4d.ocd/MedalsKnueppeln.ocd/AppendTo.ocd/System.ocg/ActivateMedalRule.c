// Wrapper to safely and consistently add the medal rule to a scenario in this folder.

global func ActivateMedalRule()
{
	var medal_rule_def = GetDefinition("Rule_Medals");
	if (medal_rule_def)
	{
		var medal_rule = CreateObject(medal_rule_def);
		if (!medal_rule)
			return false;
		// Do logging of medals in normal log.
		medal_rule->SetLogging(true);
		// No clunker rewards.
		medal_rule->SetRewarding(false);
		// Create the medal rewards object instead which gives mana.
		CreateObject(Env_MedalRewards);
		return true;
	}
	return false;
}