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
	
	// Tree_Coniferous->Place(4, Shape->Rectangle(0,0,1100,560)) ;
	// Tree_Coniferous->Place(8, Shape->Rectangle(0,560,1460,700) );
	
	// for(var o in FindObjects(Find_Func("IsTree")))
	// {
		// o->SetObjectLayer(o);
		// o.Plane = 300;
	// }
	
	// CreateEffect(waterfallfx, 1,1);
	
	// CreateLight(1510,1150, 100, 0, NO_OWNER)->SetLightColor(RGB(171,30,196));
	// CreateLight(1700,1060, 100, 0, NO_OWNER)->SetLightColor(RGB(171,30,196));
	// CreateLight(2064,1019, 100, 0, NO_OWNER)->SetLightColor(RGB(171,30,196));
	// CreateLight(1527,931, 100, 0, NO_OWNER)->SetLightColor(RGB(171,30,196));
	// CreateLight(1755,858, 100, 0, NO_OWNER)->SetLightColor(RGB(171,30,196));
	// CreateLight(2091,804, 100, 0, NO_OWNER)->SetLightColor(RGB(171,30,196));
	
	// CreateLight(264,1171, 150, 0, NO_OWNER)->SetLightColor(RGB(171,30,196));
	
	// CreateLight(1776,576, 100, 0, NO_OWNER)->SetLightColor(RGB(30,171,196));
	// CreateLight(2038,604, 100, 0, NO_OWNER)->SetLightColor(RGB(30,171,196));
	// CreateLight(2038,465, 100, 0, NO_OWNER)->SetLightColor(RGB(30,171,196));
	
	// CreateLight(654,1438, 200, 0, NO_OWNER)->SetLightColor(RGB(196,30,20));
	
	// CreateObject(Time);
	
	Time->SetCycleSpeed(0);
	Time->SetTime(daytime_scenpar);
	
	// CreateObjectAbove(EnvPack_Guidepost, 1570, 672);
	// CreateObject(EnvPack_TreeTrunks, 1665, 672);
	// CreateObjectAbove(EnvPack_Scarecrow, 1553, 672);
	// CreateObjectAbove(EnvPack_Painting, 1696, 656);
	// CreateObjectAbove(EnvPack_WineBarrel, 1702, 615);
	
	var EnvPack_Painting001 = CreateObjectAbove(EnvPack_Painting, 303, 1035);
	EnvPack_Painting001->SetName("");
	var EnvPack_Painting002 = CreateObjectAbove(EnvPack_Painting, 172, 1035);
	EnvPack_Painting002->SetName("");

	var EnvPack_WineBarrel001 = CreateObjectAbove(EnvPack_WineBarrel, 302, 991);
	EnvPack_WineBarrel001->SetName("");

	var EnvPack_Guidepost2001 = CreateObjectAbove(EnvPack_Guidepost2, 911, 1330);
	EnvPack_Guidepost2001->SetName("");
	var EnvPack_Guidepost2002 = CreateObjectAbove(EnvPack_Guidepost2, 1510, 913);
	EnvPack_Guidepost2002->SetName("");

	var EnvPack_TreeTrunks001 = CreateObject(EnvPack_TreeTrunks, 281, 992);
	EnvPack_TreeTrunks001->SetName("");

	CreateObjectAbove(Column, 597, 807);
	CreateObjectAbove(Column, 208, 807);
	CreateObjectAbove(Column, 371, 806);
	CreateObject(Column, 475, 807);
	CreateObjectAbove(Column, 61, 807);
	CreateObjectAbove(Column, 926, 1031);
	CreateObjectAbove(Column, 783, 959);
	CreateObjectAbove(Column, 229, 606);
	CreateObjectAbove(Column, 445, 606);
	CreateObjectAbove(Column, 1949, 695);
	CreateObjectAbove(Column, 1781, 695);
	CreateObjectAbove(Column, 1879, 575);
	CreateObjectAbove(Column, 1842, 191);
	CreateObjectAbove(Column, 1951, 183);
	CreateObjectAbove(Column, 2147, 222);
	CreateObjectAbove(Column, 1623, 127);
	CreateObjectAbove(Column, 1519, 135);
	CreateObjectAbove(Column, 1695, 495);
	CreateObjectAbove(Column, 1579, 471);
	CreateObjectAbove(Column, 2206, 463);
	CreateObjectAbove(Column, 2070, 503);

	var Cannon001 = CreateObject(Cannon, 1524, 728);
	Cannon001->SetComDir(COMD_Left);
	Cannon001->SetR(30);
	Cannon001->SetYDir(16);
	var Cannon002 = CreateObject(Cannon, 1732, 340);
	Cannon002->SetR(30);
	Cannon002->SetYDir(16);
	CreateObjectAbove(Cannon, 1455, 963);
	var Cannon003 = CreateObject(Cannon, 1556, 119);
	Cannon003->SetR(14);
	Cannon003->SetYDir(16);

	var Bone001 = CreateObject(Bone, 269, 1006);
	Bone001->SetR(-39);
	var Bone002 = CreateObject(Bone, 298, 1007);
	Bone002->SetR(-39);
	var Bone003 = CreateObject(Bone, 297, 1007);
	Bone003->SetR(-51);
	var Bone004 = CreateObject(Bone, 269, 1006);
	Bone004->SetR(-41);
	var Bone005 = CreateObject(Bone, 1772, 695);
	Bone005->SetR(-51);
	var Bone006 = CreateObject(Bone, 269, 1006);
	Bone006->SetR(-41);
	var Bone007 = CreateObject(Bone, 1651, 775);
	Bone007->SetR(-51);
	var Bone008 = CreateObject(Bone, 269, 1006);
	Bone008->SetR(-41);

	var Skull001 = CreateObject(Skull, 1734, 579);
	Skull001->SetR(-17);
	var Skull002 = CreateObject(Skull, 2223, 797);
	Skull002->SetR(-74);
	CreateObject(Skull, 1940, 1170);
	CreateObjectAbove(Skull, 1599, 575);
	var Skull003 = CreateObject(Skull, 280, 1003);
	Skull003->SetR(-62);
	CreateObjectAbove(Skull, 1729, 1486);
	var Skull004 = CreateObject(Skull, 2045, 1486);
	Skull004->SetR(-64);
	var Skull005 = CreateObject(Skull, 1436, 1486);
	Skull005->SetR(-64);
	CreateObjectAbove(Skull, 1845, 1165);
	var Skull006 = CreateObject(Skull, 1899, 388);
	Skull006->SetR(-46);
	var Skull007 = CreateObject(Skull, 1755, 1372);
	Skull007->SetR(-64);
	Skull007->SetXDir(1);
	CreateObjectAbove(Skull, 1859, 863);
	CreateObjectAbove(Skull, 1563, 771);
	CreateObjectAbove(Skull, 2027, 577);
	var Skull008 = CreateObject(Skull, 1942, 184);
	Skull008->SetR(-96);
	
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
	var spawns = [ [1858,835], [1945,1296], [443, 1358], [302,1228], [303,960], [359, 595], [883,1172], [1461, 930], [1597, 86], [2137, 198], [777, 929], [528, 783] ];
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


