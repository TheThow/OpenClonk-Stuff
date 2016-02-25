/**
	Knüppeln.c4s

	@author KKenny / Thow
*/


func Initialize()
{
	CreateObject(Goal_BombRun);
	CreateObject(Rule_KillLogs);
	CheckScenarioParameters();
	InitClouds();
}

func BallPos()
{
	return [LandscapeWidth()/2, 230];
}

func InitClouds()
{
	for(var i = 0; i < 8; i++)
	{
		var cloud = CreateObject(DecoCloud, Random(LandscapeWidth() + 400) - 200, Random(LandscapeHeight()), -1);
		cloud->SetClrModulation(RGBa(255, 128, 0, 40));
		cloud->SetXDir(RandomX(1, 3));
	}
}


func InitializePlayer(int plr, int iX, int iY, object pBase, int iTeam)
{
	SpawnPlayer(plr, 10);
   	SetPlayerZoomByViewRange(plr, 700, 0, PLRZOOM_Direct);
   	SetPlayerZoomByViewRange(plr, 700, 0, PLRZOOM_Direct);
   	ScheduleCall(GetCrew(plr), "SelectChampion", 15, 0);
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
	
	if(GetPlayerTeam(plr) == 1)
		relaunch = CreateObjectAbove(RelaunchContainer, LandscapeWidth() / 5, LandscapeHeight() / 2, clonk->GetOwner());
	if(GetPlayerTeam(plr) == 2)
		relaunch = CreateObjectAbove(RelaunchContainer, LandscapeWidth() - LandscapeWidth() / 5, LandscapeHeight() / 2, clonk->GetOwner());
	
	relaunch->StartRelaunch(clonk);
	relaunch->SetRelaunchTime(time, true);
}

// Gamecall from LastManStanding goal, on respawning.
protected func OnPlayerRelaunch(int plr)
{
	SpawnPlayer(plr, 3);
	var clonk = GetCrew(plr);
	clonk.ChampType = GetLastChosenChampion(plr);
	clonk.ChampType->InitChamp(clonk);
	return;
}

global func GetRandomLeftSpawn()
{
	var spawns = [[325,230]];
	var rand = Random(GetLength(spawns));
	return spawns[rand];
}

global func GetRandomRightSpawn()
{
	var spawns = [[LandscapeWidth()-325,230]];
	var rand = Random(GetLength(spawns));
	return spawns[rand];
}

func OnClonkLeftRelaunch(object clonk)
{
	var pos;

	if(GetPlayerTeam(clonk->GetOwner()) == 1)
	{
		 pos = GetRandomLeftSpawn();
	}
	if(GetPlayerTeam(clonk->GetOwner()) == 2)
	{
		 pos = GetRandomRightSpawn();
	}
	clonk->SetPosition(pos[0],pos[1]);
	clonk->SpawnProtection(20);
	return;
}

func KillsToRelaunch() { return 0; }

func LeftGoalPos()
{
	return [75, 180];
}

func LeftShieldPos()
{
	return [75, 180];
}

func RightGoalPos()
{
	return [LandscapeWidth() - 75, 180];
}

func RightShieldPos()
{
	return [LandscapeWidth() - 75, 180];
}