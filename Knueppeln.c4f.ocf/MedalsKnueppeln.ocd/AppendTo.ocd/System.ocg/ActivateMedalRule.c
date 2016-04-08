// Wrapper to safely add the medal rule to a scenario in this folder.

global func ActivateMedalRule()
{
	var medal_rule_def = GetDefinition("Rule_Medals");
	if (medal_rule_def)
	{
		var medal_rule = CreateObject(medal_rule_def);
		medal_rule->SetLogging(true);
		return true;
	}
	return false;
}