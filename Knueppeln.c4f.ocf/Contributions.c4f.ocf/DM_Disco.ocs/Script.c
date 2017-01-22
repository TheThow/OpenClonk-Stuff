/**
	Knüppeln.c4s

	@author KKenny / Thow
*/

local discolightfx0 = new Effect
{
	Timer = func()
	{
		var cR, cG, cB;
		var clri = Random(360);
		var clr = HSL(clri, 250, 127);
		cR = (clr & 0xff0000) >> 16;
		cG = (clr & 0x00ff00) >> 8;
		cB = clr & 0x0000ff;
		
		var pfx =
		{
			R = cR,
			G = cG,
			B = cB,
			Alpha = PV_Linear(100,0),
			Size = PV_Linear(PV_Random(3,15),PV_Random(3,15)),
			BlitMode = GFX_BLIT_Additive,
		};
		
		CreateParticle("Flash", RandomX(0, LandscapeWidth()), RandomX(0, LandscapeHeight()), RandomX(-10,10), RandomX(-10,10), RandomX(30,90), pfx, 10);
		CreateParticle("Sparkle", RandomX(0, LandscapeWidth()), RandomX(0, LandscapeHeight()), RandomX(-10,10), RandomX(-10,10), RandomX(30,90), pfx, 10);
		CreateParticle("MagicSpark", RandomX(0, LandscapeWidth()), RandomX(0, LandscapeHeight()), RandomX(-10,10), RandomX(-10,10), RandomX(30,90), pfx, 10);
		CreateParticle("MagicFire", RandomX(0, LandscapeWidth()), RandomX(0, LandscapeHeight()), RandomX(-10,10), RandomX(-10,10), RandomX(30,90), pfx, 10);
		CreateParticle("MagicRing", RandomX(0, LandscapeWidth()), RandomX(0, LandscapeHeight()), RandomX(-10,10), RandomX(-10,10), RandomX(30,90), pfx, 10);
		
		if ((this.Time % 10 == 0) && (this.Time > 1050 )) // Light after 30 Secs
		{
			CreateLight(RandomX(0,LandscapeWidth()), RandomX(0,LandscapeHeight()), 50, 1, nil, 0, 50)->SetLightColor(RGB(RandomX(0,120),RandomX(0,120),RandomX(0,120)));
			CreateLight(RandomX(0,LandscapeWidth()), RandomX(0,LandscapeHeight()), 20, 1, nil, 0, 50)->SetLightColor(RGB(RandomX(0,120),RandomX(0,120),RandomX(0,120)));
			CreateLight(RandomX(0,LandscapeWidth()), RandomX(0,LandscapeHeight()), 50, 1, nil, 0, 50)->SetLightColor(RGB(RandomX(0,120),RandomX(0,120),RandomX(0,120)));
			CreateLight(RandomX(0,LandscapeWidth()), RandomX(0,LandscapeHeight()), 20, 1, nil, 0, 50)->SetLightColor(RGB(RandomX(0,120),RandomX(0,120),RandomX(0,120)));
		}
	}
};


func Initialize()
{
	CreateObject(Goal_LastManStanding);
	CreateObject(Rule_KillLogs);
	CheckScenarioParameters();
	//InitClouds();
	var fxd = CreateEffect(discolightfx0, 1, 2);
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

func GetItemCratePos()
{
	for (var i = 0; i < 100; i++)
	{
		var pos = [200 + Random(LandscapeWidth() - 400), 10 + Random(LandscapeHeight() - 450)];
		
		if(GBackSolid(pos[0], pos[1]))
			continue;
		
		return pos;
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
	var spawns = [[131,181],[734,181],[922,181],[1014,181],[1322,181],[1183,302],[1000,430],[1350,430],[320,540],[600,540],[820,646],[373,800],[700,800],[580,940],[780,940],[875,1102],[1060,1102],[1309,995],[1245,808],[1314,737]];
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


