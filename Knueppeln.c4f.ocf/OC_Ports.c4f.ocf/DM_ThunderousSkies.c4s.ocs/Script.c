/*-- 
	Thunderous Skies
	Author: Mimmo_O
	
	King of the hill high in the skies.
--*/


static ThunderousSkies_air_particles, ThunderousSkies_air_particles_red;

protected func Initialize()
{
	// Goal.
	CreateObject(Rule_ObjectFade)->DoFadeTime(8 * 36);
	CreateObject(Goal_LastManStanding, 450, 380, NO_OWNER);
	CreateObject(Rule_KillLogs);
	CreateObject(Rule_Gravestones);
	
	//Enviroment.
	//SetSkyAdjust(RGBa(250,250,255,128),RGB(200,200,220));
	SetSkyParallax(1, 20,20, 0,0, nil, nil);
	Sound("Environment::BirdsLoop",true,100,nil,+1);
		
	CreateObjectAbove(Column,650,379);
	CreateObjectAbove(Column,350,409);
	CreateObjectAbove(Column,160,229);
	CreateObjectAbove(Column,448,269);
	CreateObjectAbove(Column,810,179);
	
	AddEffect("ChanneledWind", nil, 1, 1, nil);
	AddEffect("Balloons", nil, 100, 100, nil);
	
	// Moving bricks.
	var brick;
	brick = CreateObjectAbove(MovingBrick,370,424);
	brick->MoveHorizontal(344, 544);
	brick = CreateObjectAbove(MovingBrick,550,250);
	brick->MoveVertical(240, 296);

	// Smooth brick edges.
	DrawMaterialTriangle("Brick-brick", 380, 412, 0);
	
	PlaceGras();
	
	ThunderousSkies_air_particles =
	{
		Prototype = Particles_Air(),
		Size = PV_KeyFrames(0, 0, 0, 100, PV_Random(2, 5), 1000, 0),
		Stretch = PV_Speed(2000, 0),
		OnCollision = PC_Stop(),
		ForceY = -20,
		Attach = ATTACH_Back | ATTACH_MoveRelative
	};
	
	ThunderousSkies_air_particles_red =
	{
		Prototype = ThunderousSkies_air_particles,
		R = 255,
		G = PV_KeyFrames(0, 0, 255, 250, 255, 500, 0),
		B = PV_KeyFrames(0, 0, 255, 250, 255, 500, 0)
	};
	
	CheckScenarioParameters();
	
	return;
}

public func ApplyChanneledWindEffects(x, y, w, h, bottom)
{
	for(var obj in FindObjects(Find_InRect(x, y, w, h)))
	{
		obj->SetYDir(Max(obj->GetYDir()-5,-50));
		var x_dir = -1;
		if (obj->GetXDir() > 0)
			x_dir = +1;
		else if (obj->GetXDir() == 0)
			x_dir = RandomX(-2, 2);
		obj->SetXDir(obj->GetXDir() + x_dir);
	}
	CreateParticle("Air", x+Random(w),bottom,RandomX(-1,1),-30, PV_Random(10, 30), ThunderousSkies_air_particles);
}

global func FxChanneledWindTimer()
{
	Scenario->ApplyChanneledWindEffects(230, 300, 40, 90, 398);
	Scenario->ApplyChanneledWindEffects(700, 250, 60, 100, 348);
}

global func FxBalloonsTimer()
{
	if(ObjectCount(Find_ID(TargetBalloon)) > 2 )
	{
		return 1;
	}
	if(ObjectCount(Find_ID(TargetBalloon)) )
	{
		if(Random(6)) return 1;	
	}

	if(Random(2)) return 1;
	
	var x = Random(300)+50;
	if(Random(2)) x = LandscapeWidth() - x;
	var y = Random(50) + 100;
	var target;
	
	var r = Random(2);
	if (r == 0) { target = CreateObjectAbove(Boompack, x, y, NO_OWNER); target->SetR(180); }
	if (r == 1) { target = CreateObjectAbove(DynamiteBox, x, y, NO_OWNER); target->SetR(180); }
		
		
	var balloon = CreateObjectAbove(TargetBalloon, x, y-30, NO_OWNER);
	balloon->SetProperty("load",target);
	target->SetAction("Attach", balloon);
	CreateParticle("Flash", x, y, 0, 0, 8, Particles_Flash());
	AddEffect("HorizontalMoving", balloon, 1, 1, balloon);
	balloon->SetXDir(((Random(2)*2)-1) * (Random(4)+3));
}

global func PlaceGras()
{
	var x=[747, 475, 474, 359, 244, 216, 324, 705, 754, 758, 828, 828, 235, 266, 266, 269, 177, 194, 204, 223, 348, 273, 284, 365, 369, 375, 379, 285, 281, 274, 233, 390, 229, 401, 388, 414, 476, 468, 463, 457, 422, 482, 493, 615, 609, 625, 631, 700, 704, 687, 761, 763, 771, 777, 619, 615, 621, 696, 789, 766, 356, 228, 188];
	var y=[280, 376, 384, 299, 279, 224, 290, 244, 226, 215, 175, 169, 328, 319, 310, 327, 205, 212, 213, 227, 292, 306, 294, 409, 407, 413, 414, 399, 396, 390, 388, 264, 392, 249, 256, 247, 252, 249, 241, 246, 246, 245, 363, 347, 376, 349, 348, 348, 335, 350, 337, 346, 349, 351, 249, 255, 254, 237, 178, 212, 291, 305, 227];
	var r=[-91, -93, -93, 89, 93, 93, 88, 89, -92, -92, 88, 93, 93, -88, -87, -93, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43, 43, 46, 44, 48, -43, -48, -48, -45, -43, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -48, -44, -45, 48, 46, 48, 45, 0, 0, 0, 0, 0, 0, 0, 44, 0];
	for (var i = 0; i < GetLength(x); i++)
	{
		var edge=CreateObjectAbove(Grass, x[i], y[i] + 5, NO_OWNER);
		edge->SetR(r[i]); 
		edge->Initialize();
	}
	return 1;
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

global func BlastFree(x, y, level, cause_plr, bla) 
{
	return false;
}

global func GetRandomSpawn()
{
	var spawns = [[660,245],[455,265],[310,300],[190,220],[330,400],[650,365]];
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
