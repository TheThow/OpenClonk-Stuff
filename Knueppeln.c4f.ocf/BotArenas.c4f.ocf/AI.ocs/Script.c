/**
	AI
	Tests for the AI system.
	
	@author Maikel
*/


static script_enemy1;
static script_enemy2;

protected func Initialize()
{
	// Script player functions as an opponent
	CreateScriptPlayer("Enemy 1", 0xffff0000, nil, CSPF_FixedAttributes | CSPF_NoEliminationCheck | CSPF_Invisible);
	CreateScriptPlayer("Enemy 2", 0xff000000, nil, CSPF_FixedAttributes | CSPF_NoEliminationCheck | CSPF_Invisible);
	return;
}


/*-- Player Initialization --*/

protected func InitializePlayer(int plr)
{
	// Make all players hostile to each other.
	for (var plr1 in GetPlayers())
		for (var plr2 in GetPlayers())
			if (plr1 != plr2)
			{
				SetHostility(plr1, plr2, true, true);			
				SetHostility(plr2, plr2, true, true);
			}
			
	// Initialize script players differently.	
	if (GetPlayerType(plr) == C4PT_Script)
		return InitializeScriptPlayer(plr);
	
	// Everything visible to the observer.
	SetFoW(false, plr);
	SetPlayerZoomByViewRange(plr, LandscapeWidth(), LandscapeHeight(), PLRZOOM_Direct | PLRZOOM_Set | PLRZOOM_LimitMax);
	var container = CreateObject(RelaunchContainer, LandscapeWidth() / 2, LandscapeHeight() / 2);
	GetCrew(plr)->Enter(container);
	
	// Add test control effect.
	var fx = AddEffect("IntTestControl", nil, 100, 2);
	fx.testnr = 1;
	fx.launched = false;
	fx.plr = plr;
	return;
}

protected func InitializeScriptPlayer(int plr)
{
	// Remove old crew.
	var index = 0;
	while (GetCrew(plr, index))
	{
		GetCrew(plr, index)->RemoveObject();
		index++;
	}
	
	// Store the script player number.
	if (script_enemy1 == nil)
		script_enemy1 = plr;
	else if (script_enemy2 == nil)
		script_enemy2 = plr;
	return;
}

global func CreateEnemy(id champ_type, int x, int y, int plr)
{
	var enemy = CreateObjectAbove(Clonk, x, y, plr);
	if (!enemy)
		return nil;
	enemy->SetDir(DIR_Right);
	enemy->MakeCrewMember(plr);
	champ_type->InitChamp(enemy);
	enemy.ChampType = champ_type;
	enemy->CreateContents(Sword);
	enemy->SetSkin(Random(4));
	enemy->DoMagicEnergy(enemy.MaxMagic / 1000);
	KnueppelnAI->AddAI(enemy);
	KnueppelnAI->SetMaxAggroDistance(enemy, LandscapeWidth());
	KnueppelnAI->SetGuardRange(enemy, 0, 0, LandscapeWidth(), LandscapeHeight());
	enemy->AddEnergyBar();
	return enemy;
}


/*-- AI Tests --*/

global func Test9_OnStart(int plr)
{
	CreateEnemy(FireMan, 120, 258, script_enemy1);
	CreateEnemy(EarthMan, 392, 258, script_enemy2);
	// Log what the test is about.
	Log("AI battle: fire man (p1) vs. earth man (p2).");
	return true;
}

global func Test2_OnStart(int plr)
{
	CreateEnemy(IceMan, 120, 258, script_enemy1);
	CreateEnemy(ElectroMan, 392, 258, script_enemy2);
	// Log what the test is about.
	Log("AI battle: ice man (p1) vs. electro man (p2).");
	return true;
}

global func Test3_OnStart(int plr)
{
	CreateEnemy(FireMan, 120, 258, script_enemy1);
	CreateEnemy(BatMan, 120, 258, script_enemy1);
	CreateEnemy(FlashMan, 392, 258, script_enemy2);
	CreateEnemy(ElectroMan, 392, 258, script_enemy2);
	// Log what the test is about.
	Log("AI battle: fire/bat man (p1) vs. electro/flash man (p2).");
	return true;
}

global func Test4_OnStart(int plr)
{
	CreateEnemy(FireMan, 120, 258, script_enemy1);
	CreateEnemy(NinjaMan, 392, 258, script_enemy2);
	CreateEnemy(NinjaMan, 392, 258, script_enemy2);
	// Log what the test is about.
	Log("AI battle: fire man (p1) vs. ninja man (2x) (p2).");
	return true;
}

global func Test5_OnStart(int plr)
{
	CreateObject(Rule_NoFriendlyFire);
	CreateEnemy(BatMan, 120, 258, script_enemy1);
	CreateEnemy(BatMan, 120, 258, script_enemy1);
	CreateEnemy(BatMan, 392, 258, script_enemy2);
	CreateEnemy(BatMan, 392, 258, script_enemy2);
	// Log what the test is about.
	Log("AI battle: bat man (p1) vs. bat man (p2).");
	return true;
}

global func Test6_OnStart(int plr)
{
	CreateObject(Rule_NoFriendlyFire);
	CreateEnemy(HookMan, 120, 258, script_enemy1);
	CreateEnemy(BatMan, 392, 258, script_enemy2);
	CreateEnemy(BatMan, 392, 258, script_enemy2);
	// Log what the test is about.
	Log("AI battle: hook man (p1) vs. bat man (p2).");
	return true;
}

global func Test7_OnStart(int plr)
{
	CreateObject(Rule_NoFriendlyFire);
	DrawMaterialQuad("Brick", 248, 272, 248, 172, 264, 172, 264, 272);
	CreateEnemy(ArchMan, 120, 258, script_enemy1);
	CreateEnemy(FlashMan, 392, 258, script_enemy2);
	// Log what the test is about.
	Log("AI battle: arch man (p1) vs. flash man (p2).");
	return true;
}

global func Test8_OnStart(int plr)
{
	CreateObject(Rule_NoFriendlyFire);
	CreateEnemy(BallsMan, 120, 258, script_enemy1);
	CreateEnemy(BombMan, 392, 258, script_enemy2);
	// Log what the test is about.
	Log("AI battle: balls man (p1) vs. bomb man (p2).");
	return true;
}

global func Test1_OnStart(int plr)
{
	CreateObject(Rule_NoFriendlyFire);
	CreateEnemy(LaserMan, 120, 258, script_enemy1);
	CreateEnemy(GreatLeaderMan, 392, 258, script_enemy2);
	// Log what the test is about.
	Log("AI battle: laser man (p1) vs. great leader man (p2).");
	return true;
}


/*-- Test Control --*/

// Aborts the current test and launches the specified test instead.
global func LaunchTest(int nr)
{
	// Get the control effect.
	var fx = GetEffect("IntTestControl", nil);
	if (!fx)
	{
		// Create a new control effect and launch the test.
		fx = AddEffect("IntTestControl", nil, 100, 2);
		fx.testnr = nr;
		fx.launched = false;
		fx.plr = GetPlayerByIndex(0, C4PT_User);
		return;
	}
	// Finish the currently running test.
	ClearCurrentTest();
	// Start the requested test by just setting the test number and setting 
	// effect.launched to false, effect will handle the rest.
	fx.testnr = nr;
	fx.launched = false;
	return;
}

// Calling this function skips the current test, does not work if last test has been ran already.
global func SkipTest()
{
	// Get the control effect.
	var fx = GetEffect("IntTestControl", nil);
	if (!fx)
		return;
	// Finish the previous test.
	ClearCurrentTest();
	// Start the next test by just increasing the test number and setting 
	// effect.launched to false, effect will handle the rest.
	fx.testnr++;
	fx.launched = false;
	return;
}


/*-- Test Effect --*/

global func FxIntTestControlStart(object target, effect fx, int temporary)
{
	if (temporary)
		return FX_OK;
	// Set default interval.
	fx.Interval = 2;
	return FX_OK;
}

global func FxIntTestControlTimer(object target, effect fx)
{
	// Launch new test if needed.
	if (!fx.launched)
	{
		// Log test start.
		Log("=====================================");
		Log("Test %d started:", fx.testnr);
		// Start the test if available, otherwise finish test sequence.
		if (!Call(Format("~Test%d_OnStart", fx.testnr), fx.plr))
		{
			Log("Test %d not available, the previous test was the last test.", fx.testnr);
			Log("=====================================");
			Log("All tests have been successfully completed!");
			fx.testnr = 1;
			return FX_OK;
			//return FX_Execute_Kill;
		}
		fx.launched = true;
	}		
	// Check whether the current test has been finished.
	if (IsTestCompleted())
	{
		fx.launched = false;
		// Log result and increase test number.
		Log("Test %d successfully completed (p%d wins).", fx.testnr, GetTestWinner());
		// Clear current test.
		ClearCurrentTest();
		fx.testnr++;
	}
	return FX_OK;
}

global func ClearCurrentTest()
{
	RemoveAll(Find_Owner(script_enemy1));
	RemoveAll(Find_Owner(script_enemy2));
	RemoveAll(Find_ID(Rule_NoFriendlyFire));
}

global func IsTestCompleted()
{
	if (ObjectCount(Find_OCF(OCF_Alive), Find_Owner(script_enemy1)) == 0 || ObjectCount(Find_OCF(OCF_Alive), Find_Owner(script_enemy2)) == 0)
		return true;
	return false;
}

global func GetTestWinner()
{
	if (ObjectCount(Find_OCF(OCF_Alive), Find_Owner(script_enemy1)) > ObjectCount(Find_OCF(OCF_Alive), Find_Owner(script_enemy2)))
		return 1;
	return 2;
}
