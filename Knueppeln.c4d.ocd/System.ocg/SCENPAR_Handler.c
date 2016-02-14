
//ManaReg is handled in the Clonk

global func CheckScenarioParameters()
{
	if(SCENPAR_ChampSelection == 2)
		CreateObject(Rule_RandomChamp, 0, 0, -1);
		
	if(SCENPAR_ChampSelection == 3)
		CreateObject(Rule_ChampRotation, 0, 0, -1);
	
}

global func RelaunchCount()
{
	return SCENPAR_Relaunchs;
}