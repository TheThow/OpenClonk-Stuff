/**
	Wagon_Knueppeln.c4s

	@author 
*/

func Initialize()
{
	CreateObject(Goal_LastManStanding);
	CreateObject(Rule_KillLogs);
	CheckScenarioParameters();
	CreateWagons();
	AddEffect("SpawnWagon", nil, 1, 180);
	InitClouds();
}

func InitClouds()
{
	var cnt = 15;
	for(var i = 0; i < cnt; i++)
	{
		var cloud = CreateObject(DecoCloud, (LandscapeWidth() + 400)/cnt * i - 200, RandomX(LandscapeHeight() + 100, LandscapeHeight() +  10), -1);
		cloud->SetClrModulation(RGBa(255, 255, 255, 170));
		cloud->SetXDir(1);
		cloud->SetMinY(-100);
		cloud->SetMaxY(10);
		if(!Random(2))
			cloud->SetR(Random(360));
	}
}

global func FxSpawnWagonTimer(object target, proplist effect, int time)
{
	var ypos = [200,300,400];
	var y = ypos[Random(3)];
	
	var wagon = CreateObject(Wagon, 0, y, -1);
	
	if (time % (180*4) == 0)
		wagon->SetSteel();
		
	if(!Random(2))
	{
		var y2 = ypos[Random(3)];
		while(y2 == y)
			y2 = ypos[Random(3)];
		
		CreateObject(Wagon, 0, y2, -1);
	}

}

func CreateWagons()
{
	for(var y = 200; y <= 400; y+=100)
	{
		for(var x = 20; x < LandscapeWidth() - 20; x+= 38)
		{
			if(!Random(2))
			{
				var wagon = CreateObject(Wagon, x, y, -1);
				
				if(!Random(2))
					wagon->SetSteel();
			}
		}
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
	return [Random(LandscapeWidth()-40) + 20, 0];
}

func OnClonkLeftRelaunch(object clonk)
{
	var pos = GetRandomSpawn();
	clonk->SetPosition(pos[0],pos[1]);
	clonk->SpawnProtection();
	return;
}

func KillsToRelaunch() { return 0; }
