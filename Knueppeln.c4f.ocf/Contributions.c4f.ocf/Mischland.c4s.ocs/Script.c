/**
	Knüppeln.c4s

	@author KKenny / Thow
*/

static daytime_scenpar;

local waterfallfx = new Effect
{
	Timer = func()
	{
		InsertMaterial(Material("Water"), 118,1110);
	}
};

func Initialize()
{
	CreateObject(Goal_DeathMatch);
	CreateObject(Rule_KillLogs);
	CheckScenarioParameters();
	//InitClouds();
	Grass->Place(50);
	Fern->Place(30);
	Seaweed->Place(10);
	
	Tree_Coniferous->Place(4, Shape->Rectangle(0,0,1100,560)) ;
	Tree_Coniferous->Place(8, Shape->Rectangle(0,560,1460,700) );
	
	for(var o in FindObjects(Find_Func("IsTree")))
	{
		o->SetObjectLayer(o);
		o.Plane = 300;
	}
	
	CreateEffect(waterfallfx, 1,1);
	
	CreateLight(1510,1150, 100, 0, NO_OWNER)->SetLightColor(RGB(171,30,196));
	CreateLight(1700,1060, 100, 0, NO_OWNER)->SetLightColor(RGB(171,30,196));
	CreateLight(2064,1019, 100, 0, NO_OWNER)->SetLightColor(RGB(171,30,196));
	CreateLight(1527,931, 100, 0, NO_OWNER)->SetLightColor(RGB(171,30,196));
	CreateLight(1755,858, 100, 0, NO_OWNER)->SetLightColor(RGB(171,30,196));
	CreateLight(2091,804, 100, 0, NO_OWNER)->SetLightColor(RGB(171,30,196));
	
	CreateLight(264,1171, 150, 0, NO_OWNER)->SetLightColor(RGB(171,30,196));
	
	CreateLight(1776,576, 100, 0, NO_OWNER)->SetLightColor(RGB(30,171,196));
	CreateLight(2038,604, 100, 0, NO_OWNER)->SetLightColor(RGB(30,171,196));
	CreateLight(2038,465, 100, 0, NO_OWNER)->SetLightColor(RGB(30,171,196));
	
	CreateLight(654,1438, 200, 0, NO_OWNER)->SetLightColor(RGB(196,30,20));
	
	CreateObject(Time);
	
	Time->SetCycleSpeed(0);
	Time->SetTime(daytime_scenpar);
	
	CreateObjectAbove(EnvPack_Guidepost, 1570, 672);
	CreateObject(EnvPack_TreeTrunks, 1665, 672);
	CreateObjectAbove(EnvPack_Scarecrow, 1553, 672);
	CreateObjectAbove(EnvPack_Painting, 1696, 656);
	CreateObjectAbove(EnvPack_WineBarrel, 1702, 615);
	
	
	
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

func OnClonkEnteredRelaunch(object clonk, int plr)
{	
	SpawnPlayer(plr);
}

global func GetRandomSpawn()
{
	var spawns = [[280,726],[386,1172],[807,1127],[1332,1159],[1712,1063],[2057,1024],[1767,818],[1530,979],[1329,887],[1617,635],[1969,635],[1774,484],[1969,343],[611,234],[920,404],[980,220]];
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


