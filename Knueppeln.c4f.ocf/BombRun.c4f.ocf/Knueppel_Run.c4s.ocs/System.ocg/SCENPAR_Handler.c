

global func CheckScenarioParameters()
{	
	if(SCENPAR_IncreasedManaReg == 2)
		CreateObject(Rule_FastMana, 0, 0, -1);
		
	if(SCENPAR_InstaGib == 2)
		CreateObject(Rule_InstaGib, 0, 0, -1);
		
	if(SCENPAR_FriendlyFire == 2)
		CreateObject(Rule_NoFriendlyFire, 0, 0, -1);
}

global func ScoreToWin()
{
	return SCENPAR_ScoreToWin;
}