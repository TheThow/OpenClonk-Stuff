/**
	SkyCastle.c4s

	@author KKenny / Thow
*/



func Initialize()
{
	CreateObject(Goal_Domination);
	CreateObject(Rule_KillLogs);
	CheckScenarioParameters();
	SetSkyParallax(0, 15, 15, nil, nil, 1, 1);
	SetSkyAdjust(RGB(100,100,100));
	InitClouds();
	AddEffect("Raining", nil, 1, 1);
	
	// Active the medal rule if loaded.
	ActivateMedalRule();
}

global func FxRainingTimer()
{
	CastPXS("Water", 3, 5, Random(LandscapeWidth()), 0);
}

func GetItemCratePos()
{
	return [100 + Random(LandscapeWidth() - 200), 0];
}

func InitClouds()
{
	var cnt = 15;
	for(var i = 0; i < cnt; i++)
	{
		var cloud = CreateObject(DecoCloud, (LandscapeWidth() + 400)/cnt * i - 200, RandomX(-100, 10), -1);
		cloud->SetClrModulation(RGBa(30, 30, 30, 170));
		cloud->SetXDir(1);
		cloud->SetMinY(-100);
		cloud->SetMaxY(10);
		if(!Random(2))
			cloud->SetR(Random(360));
	}
}

func InitializePlayer(int plr, int iX, int iY, object pBase, int iTeam)
{
	SpawnPlayer(plr, 5);
   	SetPlayerZoomByViewRange(plr, 700, 0, PLRZOOM_Direct);
   	SetPlayerZoomByViewRange(plr, 700, 0, PLRZOOM_Direct);
   	DoScoreboardShow(1, plr);
	return;
}

func SpawnPlayer(int plr, int time)
{
	var clonk = GetCrew(plr);
	if(!clonk->FindContents(Sword))
		clonk->CreateContents(Sword);
	
	var con = clonk->Contained();
	if(con)
	{
		clonk->Exit();
		con->RemoveObject();
	}
		
	clonk->SetMagicEnergy(50);
	//clonk->ChooseMenu();
	
	var clonk = GetCrew(plr);
	var relaunch;
	relaunch = CreateObjectAbove(RelaunchContainer, LandscapeWidth() / 2, LandscapeHeight() / 2, clonk->GetOwner());
	
	relaunch->StartRelaunch(clonk);
	relaunch->SetRelaunchTime(time, true);
	ScheduleCall(clonk, "SelectChampion", 15, 0);
}

// Gamecall from LastManStanding goal, on respawning.
protected func OnPlayerRelaunch(int plr)
{
	var team = GetPlayerTeam(plr);
	var time = 5;
	
	for (var o in FindObjects(Find_ID(DominationPoint)))
	{
		if (o.owner == team)
			time = 7;
	}
	
	SpawnPlayer(plr, time);
	
	return;
}

global func GetRandomSpawn()
{
	var spawns = [[152, 518],[188, 262],[157, 621],[156, 709],[243, 783],[362, 976],[450, 1109],[279, 398]];
	var rand = Random(GetLength(spawns));
	
	var ret = spawns[rand];
	
	if(!Random(2))
		ret[0] = LandscapeWidth() - ret[0];
	
	return ret;
}

func OnClonkLeftRelaunch(object clonk)
{
	var pos = GetRandomSpawn();
	clonk->SetPosition(pos[0],pos[1]);
	clonk->SpawnProtection();
	return;
}

func KillsToRelaunch() { return 0; }


