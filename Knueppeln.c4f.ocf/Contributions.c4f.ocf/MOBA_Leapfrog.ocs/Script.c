/**
	Leap Away
	A smaller moba with lots of jumps for two teams.
	
	@author Maikel
*/


public func Initialize()
{
	// Rules.
	CreateObject(Rule_NoFriendlyFire);
	
	// Goal.
	var goal = CreateObject(Goal_Moba);
	var base_left = CreateObjectAbove(InventorsLab, 148, 745);
	goal->SetLeftBase(base_left);
	var base_right = CreateObjectAbove(InventorsLab, 1664 - 148, 745);	
	goal->SetRightBase(base_right);
	goal->SetLaneCount(3);
	
	// Gather statistics.
	InitStatistics(STATS_Type_All);

	// Adjust the mood, orange sky, darker feeling in general.
	var dark = 10;
	SetSkyAdjust(RGB(150, 42, 0));
	SetGamma(100 - dark, 100 - dark, 100 - dark);
	
	// Some decorative clouds.
	for (var cnt = 0; cnt < 12; cnt++)
	{
		var cloud = CreateObject(DecoCloud, 100 + Random(LandscapeWidth() - 200), 300 + Random(LandscapeHeight() - 400));
		cloud->SetClrModulation(RGBa(255, 128, 0, 40));
		cloud->SetXDir(RandomX(-3, 3));
	}
	return;
}

public func OnGoalsFulfilled()
{
	Sound("RoundEnd", true);
	ScheduleCall(nil, "GameOver", 80, 0);
	return true;
}

public func InitializePlayer(int plr, int x, int y, int base, team, extradata)
{	
	var crew = GetCrew(plr);
	var relaunch = CreateObject(RelaunchContainer, LandscapeWidth() / 2, LandscapeHeight() / 2);
	relaunch->StartRelaunch(crew);
	crew->CreateContents(Sword);
	
	ScheduleCall(nil, "PostPlayerInit", 15, 0, plr, team, crew, relaunch);
	return;
}

global func PostPlayerInit(int plr, int team, object crew, object relaunch)
{	
	var invl = FindObject(Find_ID(InventorsLab), Find_Team(team));
	
	crew->~SelectChampion();
	ScheduleCall(crew, "SelectChampion", 15, 0);
	
	relaunch->SetPosition(invl->GetX(), invl->GetY());
	return;
} 

public func InitAIObjects(int left_minion_plr, int right_minion_plr)
{	
	// AI Players created. Assign Objects to AI players. Useful when no human players joined the enemy team.
	for (var obj in FindObjects(Find_Or(Find_ID(InventorsLab), Find_ID(DefenseTower))))
	{
		if (obj->GetX() < LandscapeWidth() / 2)
			obj->SetOwner(left_minion_plr);
		else
			obj->SetOwner(right_minion_plr);
		obj->SetCategory(C4D_Living);
		obj->SetAlive(true);
		obj->AddVertex(0, obj->GetBottom() + 4);
		obj.MaxEnergy = 1000000;
		obj->DoEnergy(obj.MaxEnergy / 1000);
		obj->AddEnergyBar();
		GameCallEx("OnCreationRuleNoFF", obj);	
	}
	return;
}

public func GiveRandomAttackTarget(object attacker)
{
	// Stay inside lane.
	var lane;
	if (attacker.Lane == 0)
		lane = Find_Or(Find_InRect(attacker->AbsX(0), attacker->AbsY(0), 440, LandscapeHeight()), Find_InRect(attacker->AbsX(LandscapeWidth() - 440), attacker->AbsY(0), 440, LandscapeHeight()), Find_InRect(attacker->AbsX(0), attacker->AbsY(0), LandscapeWidth(), 632));
	else if (attacker.Lane == 1)
		lane = Find_Or(Find_InRect(attacker->AbsX(0), attacker->AbsY(0), 440, LandscapeHeight()), Find_InRect(attacker->AbsX(LandscapeWidth() - 440), attacker->AbsY(0), 440, LandscapeHeight()), Find_InRect(attacker->AbsX(0), attacker->AbsY(632), LandscapeWidth(), 128));
	else if (attacker.Lane == 2)
		lane = Find_Or(Find_InRect(attacker->AbsX(0), attacker->AbsY(0), 440, LandscapeHeight()), Find_InRect(attacker->AbsX(LandscapeWidth() - 440), attacker->AbsY(0), 440, LandscapeHeight()), Find_InRect(attacker->AbsX(0), attacker->AbsY(760), LandscapeWidth(), 400));
	// Minions and champs first.
	var target = attacker->FindObject(Find_OCF(OCF_Alive), lane, Find_Hostile(attacker->GetController()), Find_Distance(150), attacker->Find_PathFree(), Sort_Distance());
	if (target)
		return target;
	// Then find enemy towers.
	var tower = attacker->FindObject(Find_Property("IsMinionTarget"), lane, Find_Hostile(attacker->GetController()), Sort_Distance());
	return tower;
}

public func SpawnPlayer(int plr, object clonk)
{	
	clonk->CreateContents(Sword);
	clonk->SelectChampion();
	var invl = FindObject(Find_ID(InventorsLab), Find_Team(GetPlayerTeam(clonk->GetOwner())));
	var relaunch = CreateObject(RelaunchContainer, invl->GetX(), invl->GetY(), clonk->GetOwner());
	relaunch->StartRelaunch(clonk);
}
