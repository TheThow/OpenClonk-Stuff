/**
	Knüppeln.c4s

	@author KKenny / Thow
*/



func Initialize()
{
	CreateObject(Goal_LastManStanding);
	CreateObject(Rule_KillLogs);

	CheckScenarioParameters();
	InitClouds();
	ActivateMedalRule();
	
	//SetGamma(60,90,150);
	
	InitBorderWrap();
}

local BorderwrapFx = new Effect
{
	Timer = func()
	{
		for (var obj in FindObjects(Find_InRect(0, 0, 40, LandscapeHeight())))
		{
			obj->SetPosition(LandscapeWidth() - 42, obj->GetY());
		}
		
		for (var obj in FindObjects(Find_InRect(LandscapeWidth() - 40, 0, 40, LandscapeHeight())))
		{
			obj->SetPosition(42, obj->GetY());
		}
		
		for (var obj in FindObjects(Find_InRect(0, 0, LandscapeWidth(), 40)))
		{
			obj->SetPosition(obj->GetX(), LandscapeHeight() - 45);
		}
		
		for (var obj in FindObjects(Find_InRect(0, LandscapeHeight() - 40, LandscapeWidth(), 40)))
		{
			obj->SetPosition(obj->GetX(), 45);
		}
	}
};

func InitBorderWrap()
{
	CreateEffect(BorderwrapFx, 1, 1);
}

func InitClouds()
{
	for(var i = 0; i < 8; i++)
	{
		var cloud = CreateObject(DecoCloud, Random(LandscapeWidth() + 400) - 200, Random(LandscapeHeight()), -1);
		cloud->SetClrModulation(RGBa(32, 64, 230, 40));
		cloud->SetXDir(RandomX(1, 3));
	}
}

func GetItemCratePos()
{
	for (var i = 0; i < 100; i++)
	{
		var pos = [RandomX(LandscapeWidth() - 100, LandscapeWidth() + 100), 10 + Random(LandscapeHeight() - 450)];
		
		if(GBackSolid(pos[0], pos[1]))
			continue;
		
		return pos;
	}
}

func InitializePlayer(int plr, int iX, int iY, object pBase, int iTeam)
{
	//SpawnPlayer(plr);
   	SetPlayerZoomByViewRange(plr, 700, 0, PLRZOOM_Direct);
   	//SetPlayerZoomByViewRange(plr, 700, 0, PLRZOOM_Direct);
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
	var spawns = [[120,230],[904,230],[512,230],[340,230],[684,230]];
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


