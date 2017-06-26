/**
	Knüppeln.c4s

	@author KKenny / Thow
*/

static antirejoin;
static g_aiskins;

local spawns = [[80,190],[250,230],[360,230],[510,230],[660,230],[940,190]];

local clonkspawnerfx = new Effect
{
	Start = func()
	{
		this.spawndelay = 100;
		this.lastspawn = 0;
	},
	
	Timer = func()
	{
		if (Time >= (this.lastspawn + this.spawndelay))
		{
			this.lastspawn = Time;
			if (!FindObject(Find_OCF(OCF_CrewMember), Find_Owner(NO_OWNER)) && GetPlayerCount(C4PT_Script) <= 0)
				GameCall("SpawnRandomAIClonk");
		}
	},
};

local antirejoinfx = new Effect
{
	
};


public func SpawnRandomAIClonk()
{
	var aispawns = [[80,190],[250,230],[360,230],[510,230],[660,230],[940,190]];
	
	var rndpos = aispawns[Random(6)];
	var clnk = CreateObject(Clonk, rndpos[0], rndpos[1], NO_OWNER);
	
	var rndstuff = 0;
	
	if (rndstuff == 0)
	{
		clnk.ChampType = BatMan;
		clnk.ChampType->InitChamp(clnk);
	}
	
	clnk->CreateContents(Sword);
	clnk->SetSkin(Random(4));
	clnk->SetColor(RGB(Random(256),Random(256),Random(256)));
	
	KnueppelnAI->AddAI(clnk, KnueppelnAI);
	KnueppelnAI->SetGuardRange(clnk, 0, 0, LandscapeWidth(),LandscapeHeight());
}


func Initialize()
{
	CreateObject(Goal_LastManStanding);
	CreateObject(Rule_KillLogs);

	CheckScenarioParameters();
	//InitClouds();
	ActivateMedalRule();
	
	CreateEffect(clonkspawnerfx, 1, 1);
	antirejoin = CreateEffect(antirejoinfx, 100, 100);
	
	g_aiskins = [];
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
	return [100 + Random(LandscapeWidth() - 200), 50];
}

func InitializePlayer(int plr, int iX, int iY, object pBase, int iTeam)
{
	if (antirejoin.Time > 30)
	{
		Log("No rejoin, %s", GetPlayerName(plr));
		Sound("norejoin?");
		EliminatePlayer(plr);
		return;
	}
	//SpawnPlayer(plr);
   	SetPlayerZoomByViewRange(plr, 700, 0, PLRZOOM_Direct);
   	//SetPlayerZoomByViewRange(plr, 700, 0, PLRZOOM_Direct);
	
	if (GetPlayerType(plr) == C4PT_Script)
	{
		g_aiskins[plr] = Random(3);
	}
	
	return;
}

func SpawnPlayer(int plr, prev_clonk)
{
	var clonk = GetCrew(plr);
	clonk->CreateContents(Sword);
	clonk->SetMagicEnergy(50);
	ScheduleCall(clonk, "SelectChampion", 15, 0);
	if (GetPlayerType(plr) == C4PT_Script)
	{
		KnueppelnAI->AddAI(clonk, Scenario.spawns);
		clonk->SetSkin(g_aiskins[plr]);
	}
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
	//var spawns = [[80,190],[250,230],[360,230],[510,230],[660,230],[940,190]];
	var rand = Random(GetLength(Scenario.spawns));
	return Scenario.spawns[rand];
}

func OnClonkLeftRelaunch(object clonk)
{
	var pos = GetRandomSpawn();
	clonk->SetPosition(pos[0],pos[1]);
	clonk->SpawnProtection();
	return;
}

func KillsToRelaunch() { return 0; }


