/**
	SkyCastle.c4s

	@author KKenny / Thow
*/



func Initialize()
{
	CreateObject(Goal_LastManStanding);
	CreateObject(Rule_KillLogs);
	CheckScenarioParameters();
	SetSkyParallax(0, 15, 15, nil, nil, 1, 1);
	SetSkyAdjust(RGB(100,100,100));
	InitClouds();
	AddEffect("Raining", nil, 1, 1);
}

global func FxRainingTimer()
{
	CastPXS("Water", 3, 5, Random(LandscapeWidth()), 0);
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
	//SpawnPlayer(plr);
   	SetPlayerZoomByViewRange(plr, 700, 0, PLRZOOM_Direct);
   	SetPlayerZoomByViewRange(plr, 700, 0, PLRZOOM_Direct);
	return;
}

func SpawnPlayer(int plr, prev_clonk)
{
	var clonk = GetCrew(plr);
	clonk->CreateContents(Sword);
	clonk->SetMagicEnergy(50);
	ScheduleCall(clonk, "SelectChampion", 15, 0);
	//clonk->ChooseMenu();
}

// Gamecall from LastManStanding goal, on respawning.
protected func OnPlayerRelaunch(int plr)
{
	var clonk = GetCrew(plr);
	var relaunch = CreateObjectAbove(RelaunchContainer, LandscapeWidth() / 2, LandscapeHeight() / 2, clonk->GetOwner());
	relaunch->StartRelaunch(clonk);
	
	SpawnPlayer(plr);
	
	return;
}

global func GetRandomSpawn()
{
	var spawns = [[660,300],[410,270],[180,315],[850,315],[820,405],[200,405],[410,395],[650,395]];
	var rand = Random(GetLength(spawns));
	return spawns[rand];
}

func OnClonkLeftRelaunch(object clonk)
{
	var pos = GetRandomSpawn();
	clonk->SetPosition(pos[0],pos[1]);
	clonk->SpawnProtection();
	return;
}

func KillsToRelaunch() { return 0; }


