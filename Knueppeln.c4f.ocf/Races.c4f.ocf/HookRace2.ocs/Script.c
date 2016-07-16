/**
	HookRace.c4s

	@author 
*/

static air_particles;

func Initialize()
{
	air_particles =
	{
		Prototype = Particles_Air(),
		Size = PV_KeyFrames(0, 0, 0, 100, PV_Random(2, 5), 1000, 0),
		Stretch = PV_Speed(2000, 0),
		ForceY = -20,
		Attach = ATTACH_Back | ATTACH_MoveRelative
	};

	//AddEffect("Spaaaace", nil, 1, 2);

	CreateEnvironment();

	var goal = CreateObject(Goal_Parkour, 0, 0, NO_OWNER);
	var mode = PARKOUR_CP_Respawn | PARKOUR_CP_Check | PARKOUR_CP_Ordered;
	goal->SetStartpoint(210, 1300);
	goal->AddCheckpoint(875, 1265, mode);
	goal->AddCheckpoint(750, 815, mode);
	goal->AddCheckpoint(770, 370, mode);
	goal->AddCheckpoint(1690, 145, mode);
	/*goal->AddCheckpoint(535, 1175, mode);
	goal->AddCheckpoint(570, 985, mode);
	goal->AddCheckpoint(1160, 950, mode);
	goal->AddCheckpoint(1850, 950, mode);
	goal->AddCheckpoint(2165, 1395, mode);
	goal->SetFinishpoint(3820, 1515);
	
	Grass->Place(100*(GetPlayerCount()/2), nil);
	Tree_Coniferous->Place(10);*/
	//goal->SetStartpoint(1690, 145);
}

global func FxSpaaaaceTimer(object target, proplist effect, int time)
{
	for(var o in FindObjects(Find_NoContainer()))
	{
		if(!o)
			continue;
	
		if(o->GetProcedure() == "FLOAT")
			continue;
		
		if(o->GetMaterial() != Material("SpaceTunnel"))
			o->SetYDir(o->GetYDir(100) - 31, 100);
		
	}
}


func CreateEnvironment()
{
	AddEffect("ChanneledWind", nil, 1, 1, nil);

	//Room1
	CreateObject(HookPoint, 370, 1160, -1);
	
	for(var i = 1260; i > 1060; i-=40)
		CreateObject(RaceSawBlade, 450, i, -1)->Stick();
	
	for(var i = 1275; i > 1000; i-=40)
		CreateObject(RaceSawBlade, 570, i, -1)->Stick();
	
	for(var i = 570; i <= 770; i+=40)
		CreateObject(RaceSawBlade, i, 1275, -1)->Stick();
	
	for(var i = 290; i <= 570; i+=40)
		CreateObject(RaceSawBlade, i, 995, -1)->Stick();
		
	CreateObject(RaceSawBlade, 430, 1030, -1)->Stick();

	CreateObject(Launcher, 490, 1325, -1)->Set(90, 100, MovingBrick, 20);
	
	CreateObject(Launcher, 910, 1310, -1)->Set(180, 70, InstaProjectile);
	
	//Room2
	CreateObject(RaceBlackHole, 1263, 1046, -1)->StartBlackHole(50, 300, 18);
	CreateObject(RaceSawBlade, 1263, 1046, -1)->Circle(70, 5, -1);
	CreateObject(RaceSawBlade, 995, 850, -1)->Stick();
	
	CreateObject(HookPoint, 1184, 927, -1);
	CreateObject(HookPoint, 1042, 877, -1);
	
	//Room3
	for(var i = 0; i < 12; i++)
		CreateObject(RaceSawBlade, 690 - i*40, 670, -1)->Stick();
	
	for(var i = 0; i < 11; i++)
		CreateObject(RaceSawBlade, 45, 450 + i*40, -1)->Stick();
	
	CreateObject(HookPoint, 155, 635, -1);
	CreateObject(HookPoint, 615, 540, -1);
	CreateObject(RaceSawBlade, 250, 450, -1)->Shizzle(0, 180, 4);
	//CreateObject(RaceSawBlade, 250, 630, -1)->Shizzle(0, -180, 4);
	CreateObject(RaceSawBlade, 615, 540, -1)->Circle(40, 2, -1);
	CreateObject(RaceSawBlade, 615, 540, -1)->Circle(40, 2, -1, 180);
	
	
	//Room4
	CreateObject(RaceSawBlade, 900, 585, -1)->Stick();
	CreateObject(RaceSawBlade, 923, 550, -1)->Stick();
	CreateObject(RaceSawBlade, 945, 585, -1)->Stick();
	
	CreateObject(RaceSawBlade, 973, 510, -1)->Stick();
	
	CreateObject(RaceSawBlade, 1000, 585, -1)->Stick();
	CreateObject(RaceSawBlade, 1023, 550, -1)->Stick();
	CreateObject(RaceSawBlade, 1045, 585, -1)->Stick();
	
	
	CreateObject(RaceSawBlade, 1260, 550, -1)->Circle(50, 4, 1);
	CreateObject(RaceSawBlade, 1260, 550, -1)->Circle(50, 4, 1, 180);
	
	CreateObject(RaceSawBlade, 1430, 550, -1)->Shizzle(0, -100, 3);
	CreateObject(RaceSawBlade, 1430, 590, -1)->Stick();
	
	//Room5
	/*CreateObject(Launcher, 1776, 175, -1)->Set(180, 110, MovingBrick, 15);
	CreateObject(Launcher, 1819, 175, -1)->Set(180, 110, MovingBrick, 15);
	CreateObject(Launcher, 1862, 175, -1)->Set(180, 110, MovingBrick, 15);
	CreateObject(Launcher, 1905, 175, -1)->Set(180, 110, MovingBrick, 15);*/
	
	CreateObject(RaceSawBlade, 1840, 325, -1)->Circle(70, 4, 1);
	CreateObject(RaceSawBlade, 1840, 325, -1)->Circle(70, 4, 1, 180);
	
	CreateObject(RaceSawBlade, 1770, 540, -1)->Shizzle(145, 0, 4);
	
	CreateObject(Launcher, 1700, 677, -1)->Set(0, 30, InstaProjectile);
	CreateObject(Launcher, 1980, 677, -1)->Set(180, 30, InstaProjectile);
	CreateObject(Launcher, 1700, 781, -1)->Set(0, 30, InstaProjectile);
	CreateObject(Launcher, 1980, 846, -1)->Set(180, 30, InstaProjectile);
	
	
	/*CreateObject(RaceSawBlade, 1825, 945, -1)->Stick();
	CreateObject(RaceSawBlade, 1865, 945, -1)->Stick();
	
	CreateObject(RaceSawBlade, 1775, 1025, -1)->Stick();
	CreateObject(RaceSawBlade, 1815, 1025, -1)->Stick();
	CreateObject(RaceSawBlade, 1910, 1025, -1)->Stick();
	CreateObject(RaceSawBlade, 1870, 1025, -1)->Stick();
	
	CreateObject(RaceSawBlade, 1825, 1105, -1)->Stick();
	CreateObject(RaceSawBlade, 1865, 1105, -1)->Stick();*/
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

global func FxChanneledWindTimer()
{
	ApplyChanneledWindEffects(810, 830, 50, 85);
	
	//Room3
	ApplyChanneledWindEffects(560, 775, 50, 85);
	ApplyChanneledWindEffects(420, 775, 50, 85);
	ApplyChanneledWindEffects(280, 775, 50, 85);
	
	ApplyChanneledWindEffects(395, 570, 100, 85);
	ApplyChanneledWindEffects(750, 560, 665, 85);
	ApplyChanneledWindEffects(715, 570, 25, 150);
	
	//Room4
	ApplyChanneledWindEffects(1560, 155, 60, 450);
}

global func ApplyChanneledWindEffects(x, y, w, h, bottom)
{
	for(var obj in FindObjects(Find_InRect(x, y, w, h)))
	{
		obj->SetYDir(Max(obj->GetYDir()-4,-50));
		/*if(GetXDir())
			x_dir = GetXDir() / Abs(GetXDir());
		obj->SetXDir(obj->GetXDir() + x_dir);*/
	}
	CreateParticle("Air", x+Random(w),y+h,RandomX(-1,1),-30, PV_Random(10, 30), air_particles);
}
