/**
	SkyCastle.c4s

	@author KKenny / Thow
*/

local team_coins_earned;
local player_coins_earned;

local player_bonus_hp;

func Initialize()
{
	player_bonus_hp = [];
	player_coins_earned = [];
	team_coins_earned = [];
	
	CreateObject(Goal_Domination);
	CreateObject(Rule_KillLogs);
	CheckScenarioParameters();
	SetSkyParallax(0, 15, 15, nil, nil, 1, 1);
	SetSkyAdjust(RGB(100,100,100));
	InitClouds();
	AddEffect("Raining", nil, 1, 1);
	
	// Active the medal rule if loaded.
	ActivateMedalRule();
	
	Scoreboard->Init(
		[
			{key = "coins_earned", title = Nugget, sorted = true, desc = true, priority = 90, default = ""},
		]
		);
	GUI_Controller_Wealth->ShowWealth();
}

public func DoPlayerCoins(int plr, int coins)
{
	if (!GetPlayerName(plr)) return;
	player_coins_earned[plr] += coins;
	var team_id = Goal_Domination->CalculateTeamID(GetPlayerTeam(plr));
	team_coins_earned[team_id] += coins;
	Scoreboard->SetPlayerData(plr, "coins_earned", player_coins_earned[plr]);
	Scoreboard->SetData(team_id, "coins_earned", team_coins_earned[team_id]);
	
	Sound("UI::UnCash*", {player = plr});
	DoWealth(plr, coins);
}

public func OnClonkDeath(object clonk, int killer)
{
	var plr = clonk->GetOwner();
	if (GetPlayerName(plr) && Hostile(plr, killer))
		DoPlayerCoins(killer, 2);
	return _inherited(clonk, killer, ...);
}

public func OnCapturePointCaptured(object cp, int team)
{
	for (var clonk in FindObjects(Find_Distance(50, cp->GetX(), cp->GetY()), Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive), Find_NoContainer(), Find_Func("IsClonk")))
	{
		var owner = clonk->GetOwner();
		if (GetPlayerTeam(owner) == team)
			DoPlayerCoins(owner, 5);
	}
}

public func OnCapturePointLiberated(object cp, int team, int old_team)
{
	for (var clonk in FindObjects(Find_Distance(50, cp->GetX(), cp->GetY()), Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive), Find_NoContainer(), Find_Func("IsClonk")))
	{
		var owner = clonk->GetOwner();
		if (GetPlayerTeam(owner) == team)
			DoPlayerCoins(owner, 4);
	}
}

global func FxRainingTimer()
{
	CastPXS("Water", 3, 5, Random(LandscapeWidth()), 0);
}

func GetItemCratePos()
{
	return [100 + Random(LandscapeWidth() - 200), 0];
}

func InitClouds()
{
	var cnt = 15;
	for(var i = 0; i < cnt; i++)
	{
		var cloud = CreateObject(DecoCloud, (LandscapeWidth() + 400)/cnt * i - 200, RandomX(-100, 10), -1);
		cloud->SetClrModulation(RGBa(30, 30, 30, 170));
		cloud->SetXDir(1);
		cloud->SetMinY(-100);
		cloud->SetMaxY(10);
		if(!Random(2))
			cloud->SetR(Random(360));
	}
}

func InitializePlayer(int plr, int iX, int iY, object pBase, int iTeam)
{
	SpawnPlayer(plr, 5);
   	SetPlayerZoomByViewRange(plr, 700, 0, PLRZOOM_Direct);
   	SetPlayerZoomByViewRange(plr, 700, 0, PLRZOOM_Direct);
   	DoScoreboardShow(1, plr + 1);
   	CreateObject(Homebase, 0,0, plr);
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
	relaunch = CreateObjectAbove(RelaunchContainer, LandscapeWidth() / 2, LandscapeHeight() / 2, clonk->GetOwner());
	
	relaunch->StartRelaunch(clonk);
	relaunch->SetRelaunchTime(time, true);
	ScheduleCall(clonk, "SelectChampion", 15, 0);
}

// Gamecall from LastManStanding goal, on respawning.
protected func OnPlayerRelaunch(int plr)
{
	var team = GetPlayerTeam(plr);
	var time = 5;
	
	for (var o in FindObjects(Find_ID(DominationPoint)))
	{
		if (o.owner == team)
			time = 7;
	}
	
	SpawnPlayer(plr, time);
	
	return;
}

global func GetRandomSpawn()
{
	var spawns = [[152, 518],[188, 262],[157, 621],[156, 709],[243, 783],[362, 976],[450, 1109],[279, 398]];
	var rand = Random(GetLength(spawns));
	
	var ret = spawns[rand];
	
	if(!Random(2))
		ret[0] = LandscapeWidth() - ret[0];
	
	return ret;
}

func OnClonkLeftRelaunch(object clonk)
{
	var pos = GetRandomSpawn();
	clonk->SetPosition(pos[0],pos[1]);
	clonk->SpawnProtection();
	
	ApplyBonusHP(clonk);
	
	return;
}

func ApplyBonusHP(object clonk)
{
	if (!clonk) return;
	var plr = clonk->GetOwner();
	if (GetLength(player_bonus_hp) > plr && player_bonus_hp[plr] != nil)
	{
		clonk.MaxEnergy = Clonk.MaxEnergy + 1000 * player_bonus_hp[plr];
		clonk->DoEnergy(player_bonus_hp[plr]);
	}
}

func KillsToRelaunch() { return 0; }

// Enter all buyable things into the homebase
func FillHomebase(object homebase)
{	
	// Buy menu entries
	homebase->AddCaption("Items");
	for (var thing in [[Boompack, 5], [MonsterBall, 2], [RubberDucky, 5], [BlackHole, 10], [Superberry, 10]]) 
		homebase->AddHomebaseItem(new Homebase.ITEMTYPE_Consumable { item = thing[0], cost = thing[1], desc = thing[0].Description });

	//homebase->AddCaption("$HomebaseTechnology$");
	//homebase->AddHomebaseItem(new Homebase.ITEMTYPE_Technology { name="$HomebaseAdvancedWeapons$", item = Icon_World,cost = 100, desc="$HomebaseDescAdvancedWeapons$", tech = "AdvancedWeapons" });
	//homebase->AddHomebaseItem(new Homebase.ITEMTYPE_Technology { name="$HomebaseMasterWeapons$", item = Icon_World,cost = 1000, desc = "$HomebaseDescMasterWeapons$", tech = "MasterWeapons", requirements = ["AdvancedWeapons"] });

	homebase->AddCaption("$HomebaseUpgrades$");
	//homebase->AddHomebaseItem(new Homebase.ITEMTYPE_Technology { name="$HomebaseLoadSpeed$", item = Homebase_Icon, graphics="LoadSpeed%d", costs = [100, 500, 1000], desc = "$HomebaseDescLoadSpeed$", tech = "LoadSpeed", tiers=3 });
	homebase->AddHomebaseItem(new Homebase.ITEMTYPE_Technology { name="$HomebaseLife$", item = Homebase_Icon, graphics="Life%d", costs = [15, 20, 40], desc = "$HomebaseDescLife$", tech = "Life", tiers=3 });
	
	homebase.GainLife = this.OnHomebaseGainLife;
}

public func OnHomebaseGainLife(proplist entry, int tier)
{
	Scenario.player_bonus_hp[this->GetOwner()] = [10, 20, 50][tier - 1];
	Scenario->ApplyBonusHP(GetCrew(this->GetOwner()));
}

func RemovePlayer(int plr)
{
	if (g_homebases[plr]) g_homebases[plr]->RemoveObject();
	return;
}
