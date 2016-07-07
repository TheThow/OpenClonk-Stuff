/**
	HookRace.c4s

	@author 
*/

func Initialize()
{
	CreateEnvironment();

	var goal = CreateObject(Goal_Parkour, 0, 0, NO_OWNER);
	var mode = PARKOUR_CP_Respawn | PARKOUR_CP_Check | PARKOUR_CP_Ordered;
	// goal->SetStartpoint(90, 1510);
	// goal->AddCheckpoint(520, 1510, mode);
	// goal->AddCheckpoint(1420, 1510, mode);
	// goal->AddCheckpoint(1460, 1175, mode);
	// goal->AddCheckpoint(535, 1175, mode);
	// goal->AddCheckpoint(570, 985, mode);
	// goal->AddCheckpoint(1160, 950, mode);
	// goal->AddCheckpoint(1850, 950, mode);
	// goal->AddCheckpoint(2165, 1395, mode);
	// goal->SetFinishpoint(3820, 1515);
	
	goal->SetStartpoint(3190, 1521);
	goal->AddCheckpoint(2165, 1395, mode);
	goal->AddCheckpoint(1850, 950, mode);
	goal->AddCheckpoint(1160, 950, mode);
	goal->AddCheckpoint(570, 985, mode);
	goal->AddCheckpoint(535, 1175, mode);
	goal->AddCheckpoint(1460, 1175, mode);
	goal->AddCheckpoint(1420, 1510, mode);
	goal->AddCheckpoint(520, 1510, mode);
	goal->SetFinishpoint(90, 1510);
	
	
	Grass->Place(100, nil);
	Tree_Coniferous->Place(5);
	Tree_Coniferous2->Place(5);
	Tree_Deciduous->Place(5);
	Mushroom->Place(20);
	Flower->Place(30);
	Fern->Place(15);
}



func CreateEnvironment()
{
	//Room1
	CreateObject(HookPoint, 290, 1475, -1);
	CreateObject(RaceSawBlade, 460, 1450, -1)->Stick();
	
	//Room2
	CreateObject(HookPoint, 660, 1460, -1);
	CreateObject(HookPoint, 1020, 1463, -1);
	CreateObject(HookPoint, 1255, 1500, -1);
	
	CreateObject(RaceSawBlade, 760, 1380, -1)->Stick();
	CreateObject(RaceSawBlade, 760, 1420, -1)->Stick();
	CreateObject(RaceSawBlade, 760, 1490, -1)->Stick();
	CreateObject(RaceSawBlade, 760, 1530, -1)->Stick();
	
	//Room3
	CreateObject(HookPoint, 1500, 1460, -1);
	CreateObject(HookPoint, 1675, 1460, -1);
	CreateObject(HookPoint, 1590, 1210, -1);
	
	CreateObject(RaceSawBlade, 1590, 1310, -1)->SetXDir(-1);
	CreateObject(RaceSawBlade, 1590, 1310, -1)->SetXDir(1);
	CreateObject(RaceSawBlade, 1620, 1240, -1)->Stick();
	CreateObject(RaceSawBlade, 1555, 1240, -1)->Stick();
	
	//Room4
	CreateObject(RaceSawBlade, 700, 1200, -1)->Snap(0);
	CreateObject(RaceSawBlade, 740, 1200, -1)->Snap(0);
	//CreateObject(RaceSawBlade, 860, 1200, -1)->Snap(0);
	CreateObject(RaceSawBlade, 700, 1230, -1)->Snap(180);
	CreateObject(RaceSawBlade, 740, 1230, -1)->Snap(180);
	
	CreateObject(RaceSawBlade, 950, 1200, -1)->Snap(0);
	CreateObject(RaceSawBlade, 990, 1200, -1)->Snap(0);
	CreateObject(RaceSawBlade, 1030, 1200, -1)->Snap(0);
	CreateObject(RaceSawBlade, 1070, 1200, -1)->Snap(0);
	CreateObject(RaceSawBlade, 950, 1230, -1)->Snap(180);
	CreateObject(RaceSawBlade, 990, 1230, -1)->Snap(180);
	CreateObject(RaceSawBlade, 1030, 1230, -1)->Snap(180);
	CreateObject(RaceSawBlade, 1070, 1230, -1)->Snap(180);
	
	CreateObject(RaceSawBlade, 1200, 1200, -1)->Snap(0);
	CreateObject(RaceSawBlade, 1240, 1200, -1)->Snap(0);
	CreateObject(RaceSawBlade, 1200, 1230, -1)->Snap(180);
	CreateObject(RaceSawBlade, 1240, 1230, -1)->Snap(180);
	//CreateObject(RaceSawBlade, 1360, 1230, -1)->Snap(180);
	
	//Room5
	CreateObject(HookPoint, 180, 1080, -1);
	CreateObject(HookPoint, 230, 930, -1);
	//CreateObject(HookPoint, 350, 975, -1);
	CreateObject(HookPoint, 450, 960, -1);
	
	CreateObject(RaceSawBlade, 400, 1180, -1)->SetXDir(1);
	CreateObject(RaceSawBlade, 300, 1150, -1)->SetXDir(-1);
	CreateObject(RaceSawBlade, 200, 1120, -1)->SetXDir(1);
	
	for(var i = 485; i > 225; i-=40)
		CreateObject(RaceSawBlade, i, 1045, -1)->Stick();
	
	CreateObject(RaceSawBlade, 300, 875, -1)->Stick();
	CreateObject(RaceSawBlade, 340, 875, -1)->Stick();
	CreateObject(RaceSawBlade, 380, 875, -1)->Stick();
	CreateObject(RaceSawBlade, 340, 915, -1)->Stick();
	
	//Room6
	CreateObject(Launcher, 1170, 997, -1)->Set(180, 50, InstaProjectile);
	
	for(var i = 700; i < 1100; i+=100)
		CreateObject(Launcher, i, 865, -1)->Set(90, 30, InstaProjectile);
	
	//Room7
	CreateObject(HookPoint, 1590, 850, -1);
	CreateObject(HookPoint, 1790, 935, -1);
	
	CreateObject(RaceSawBlade, 1265, 995, -1)->Shizzle(0, -60, 3);
	CreateObject(RaceSawBlade, 1335, 895, -1)->Shizzle(0, 60, 3);
	
	var bx = 1465;
	var by = 865;
	for(var i = 0; i < 120; i+=30)
		CreateObject(RaceSawBlade, bx + i, by - i, -1)->Stick();
	
	bx = 1780;
	by = 865;
	for(var i = 0; i < 120; i+=30)
		CreateObject(RaceSawBlade, bx - i, by - i, -1)->Stick();
	
	var bx = 1560;
	var by = 975;
	for(var i = 0; i < 90; i+=30)
	{
		CreateObject(RaceSawBlade, bx + i, by - i, -1)->Stick();
		CreateObject(RaceSawBlade, bx - i + 120, by - i, -1)->Stick();
	}
	
	//Room8
	CreateObject(HookPoint, 2040, 1380, -1);
	
	var bx = 1945;
	var by = 985;
	for(var i = 0; i < 80; i+=20)
		CreateObject(RaceSawBlade, bx + i, by + i*2, -1)->Stick();
		
	var bx = 2110;
	var by = 1120;
	for(var i = 0; i < 80; i+=14)
		CreateObject(RaceSawBlade, bx - i, by + i*17/7, -1)->Stick();
	
	CreateObject(RaceSawBlade, 2110, 1330, -1)->Stick();
	
	CreateObject(HookPoint, 2020, 1155, -1);
	CreateObject(HookPoint, 2030, 982, -1);
	
	
	//Room9
	CreateObject(HookPoint, 2290, 1380, -1);
	CreateObject(HookPoint, 2460, 1335, -1);
	
	CreateObject(RaceSawBlade, 2215, 1320, -1)->Shizzle(0, 125, 3);
	
	for(var i = 1260; i < 1500; i+=40)
		CreateObject(RaceSawBlade, 2550, i, -1)->Stick();
	
	CreateObject(HookPoint, 2675, 1345, -1);
	CreateObject(HookPoint, 2820, 1190, -1);
	
	CreateObject(RaceSawBlade, 2550, 1120, -1)->Shizzle(0, 100, 2);
	
	CreateObject(RaceSawBlade, 3155, 1450, -1)->Stick();
	CreateObject(RaceSawBlade, 3090, 1450, -1)->Stick();
	
	CreateObject(RaceSawBlade, 2820, 1190, -1)->Circle(40, 4, 1);
	
	CreateObject(HookPoint, 3120, 1470, -1);
	CreateObject(HookPoint, 3060, 1290, -1);
	
	CreateObject(RaceSawBlade, 3400, 1521, -1)->Stick();
	
}

func InitializePlayer(int plr, int iX, int iY, object pBase, int iTeam)
{
	//SpawnPlayer(plr);
   	SetPlayerZoomByViewRange(plr, 700, 0, PLRZOOM_Direct);
   	//SetPlayerZoomByViewRange(plr, 700, 0, PLRZOOM_Direct);
   	OnPlayerRespawn(plr);
	return;
}

protected func OnPlayerRespawn(int plr, object cp)
{
	var clonk = GetCrew(plr);
	// Jump to instantly collect the boompack.
	clonk.ChampType = RaceHookMan;
	clonk->DoEnergy(1000);
	clonk->DoEnergy(-75);
	RemoveEffect("AutoHeal", clonk);
	clonk->DoMagicEnergy(100, true);
}
