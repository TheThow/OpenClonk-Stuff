

global func CheckScenarioParameters()
{		
	if(SCENPAR_InstaGib == 2)
		CreateObject(Rule_InstaGib, 0, 0, -1);
		
	if(SCENPAR_FriendlyFire == 2)
	{
		CreateObject(Rule_NoFriendlyFire, 0, 0, -1);
	}
	
	if(SCENPAR_Items != 0)
	{
		var r = CreateObject(Rule_Items, 0, 0, -1);
		r->Set(SCENPAR_Items);
	}
}

global func ScoreToWin()
{
	return SCENPAR_ScoreToWin;
}