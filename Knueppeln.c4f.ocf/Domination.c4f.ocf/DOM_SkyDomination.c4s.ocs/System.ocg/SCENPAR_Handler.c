

global func CheckScenarioParameters()
{	
	if(SCENPAR_NodeCount == 3)
	{
		CreateObject(DominationPoint, 980, 650, -1);
		CreateObject(DominationPoint, LandscapeWidth() - 980, 650, -1);
		CreateObject(DominationPoint, LandscapeWidth()/2, 930, -1);
	}
	
	if(SCENPAR_NodeCount == 2)
	{
		CreateObject(DominationPoint, LandscapeWidth()/2, 575, -1);
		CreateObject(DominationPoint, LandscapeWidth()/2, 765, -1);
	}
	
	if(SCENPAR_NodeCount == 1)
	{
		CreateObject(DominationPoint, LandscapeWidth()/2, 765, -1);
	}

	if(SCENPAR_FriendlyFire == 2)
	{
		CreateObject(Rule_NoFriendlyFire, 0, 0, -1);
	}


	if(SCENPAR_IncreasedManaReg == 2)
		CreateObject(Rule_FastMana, 0, 0, -1);
	
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