

global func CheckScenarioParameters()
{
	if(SCENPAR_ChampSelection == 2)
		CreateObject(Rule_RandomChamp, 0, 0, -1);
		
	if(SCENPAR_ChampSelection == 3)
		CreateObject(Rule_ChampRotation, 0, 0, -1);
		
	if(SCENPAR_IncreasedManaReg == 2)
		CreateObject(Rule_FastMana, 0, 0, -1);
		
	if(SCENPAR_InstaGib == 2)
		CreateObject(Rule_InstaGib, 0, 0, -1);
	
	if(SCENPAR_Items != 0)
	{
		var r = CreateObject(Rule_Items, 0, 0, -1);
		r->Set(SCENPAR_Items);
	}
}