
/*

	Sky Fortress / Himmelsfestung
	
	@author: K-Pone

*/


static g_plrid_minions_left;
static g_plrid_minions_right;

static g_deftw1;
static g_deftw2;
static g_deftw3;
static g_deftw4;
static g_deftw5;
static g_deftw6;
static g_deftw7;
static g_deftw8;
static g_deftw9;
static g_deftw10;
static g_deftw11;
static g_deftw12;
static g_invll;
static g_invlr;

static g_goal;
static g_gameover;

local FxMinionSpawner = new Effect
{	
	Timer = func()
	{
		//Log("%v %v", Time, Target->GetOwner());
		if ((Time % 525) == 1)   SpawnMinion(Sword, "Top");
		if ((Time % 525) == 20)  SpawnMinion(Sword, "Top");
		if ((Time % 525) == 40)  SpawnMinion(Sword, "Top");
		if ((Time % 525) == 60)  SpawnMinion(Sword, "Top");
		if ((Time % 525) == 80)  SpawnMinion(Sword, "Top");
		//if ((Time % 525) == 80)  SpawnMinion(Bow,   "Top", true);
		if ((Time % 525) == 100) SpawnMinion(Sword, "Bot");
		if ((Time % 525) == 120) SpawnMinion(Sword, "Bot");
		if ((Time % 525) == 140) SpawnMinion(Sword, "Bot");
		if ((Time % 525) == 160) SpawnMinion(Sword, "Bot");
		if ((Time % 525) == 180) SpawnMinion(Sword, "Bot");
		//if ((Time % 525) == 180) SpawnMinion(Bow,   "Bot", true);
	},
	
	SpawnMinion = func(weapon, lane, towerattacker)
	{
		var minion = Target->CreateObject(Clonk);
		minion->SetOwner(Target->GetOwner());
		
		if (weapon == Bow)
			minion->CreateContents(Bow)->CreateContents(BombArrow)->SetInfiniteStackCount();
		else
			minion->CreateContents(weapon);
		minion.isMobaMinion = true;
		
		if (Target->GetOwner() == g_plrid_minions_left)
			minion->SetClrModulation(RGB(255,32,32));
		if (Target->GetOwner() == g_plrid_minions_right)
			minion->SetClrModulation(RGB(32,32,255));
		
		minion.Lane = lane;
		
		if (towerattacker) minion.isTowerattacker = true;
		
		MobaMinionAI->AddAI(minion);
		
		GameCallEx("OnCreationRuleNoFF", minion);
		
		return;
	},
};

local FxGoalCheckTimer = new Effect
{
	Timer = func()
	{
		if (g_goal->IsFulfilled())
		{
			if (!g_gameover)
			{
				g_gameover = true;
				Sound("RoundEnd", true);
				ScheduleCall(nil, "GameOver", 80, 0);
			}
		}
			
		
	}
};


func Initialize()
{
	CreateObject(Rule_NoFriendlyFire);
	
	InitStuff();
	InitMinionPlayers();
	
	g_goal = CreateObject(Goal_Moba);
	g_goal->SetLeftBase(g_invll);
	g_goal->SetRightBase(g_invlr);
	
	CreateEffect(FxGoalCheckTimer, 1, 50);

	// Gather statistics.
	InitStatistics(STATS_Type_All);	
}

func InitMinionPlayers()
{
	CreateScriptPlayer("$MinionTeamLeft$", RGB(225, 0, 0), 1, CSPF_FixedAttributes | CSPF_NoScenarioInit | CSPF_NoEliminationCheck | CSPF_Invisible, Goal_Moba);
	CreateScriptPlayer("$MinionTeamRight$", RGB(64, 64, 255), 2, CSPF_FixedAttributes | CSPF_NoScenarioInit | CSPF_NoEliminationCheck | CSPF_Invisible, Goal_Moba);
}

func InitializePlayer(plr, x, y, base, team, extradata)
{	
	var crew = GetCrew(plr);
	var relaunch = CreateObject(RelaunchContainer, LandscapeWidth() / 2, LandscapeHeight() / 2);
	relaunch->StartRelaunch(crew);
	crew->CreateContents(Sword);
	
	ScheduleCall(nil, "PostPlayerInit", 15, 0, [plr, x, y, base, team, extradata, crew, relaunch]);
	
}

global func PostPlayerInit(params)
{	
	var plr = params[0];
	var x = params[1];
	var y = params[2];
	var base = params[3];
	var team = params[4];
	var extradata = params[5];
	var crew = params[6];
	var relaunch = params[7];
	
	if (!g_plrid_minions_right)
		ScheduleCall(nil, "PostPlayerInit", 15, 0, [plr, x, y, base, team, extradata, crew, relaunch]);
	else
		Log("Right Minions is %v", g_plrid_minions_right);
	
	var invl = FindObject(Find_ID(InventorsLab),Find_Team(team));
	
	crew->~SelectChampion();
	ScheduleCall(crew, "SelectChampion", 15, 0);
	
	relaunch->SetPosition(invl->GetX(), invl->GetY());
	
	GameCallEx("OnCreationRuleNoFF", crew);
} 

func InitAIStuff()
{	
	// AI Players created. Assign Objects to AI players. Useful when no human players joined the enemy team.
				
	for (var obj in FindObjects(Find_Or(Find_ID(InventorsLab), Find_ID(DefenseTower))))
	{
		if (obj->GetX() < LandscapeWidth() / 2)
			obj->SetOwner(g_plrid_minions_left);
		else
			obj->SetOwner(g_plrid_minions_right);
		obj->SetCategory(C4D_Living);
		obj->SetAlive(true);
		obj->AddVertex(0, obj->GetBottom() + 4);
		obj.MaxEnergy = 400000;
		obj->DoEnergy(obj.MaxEnergy / 1000);
		obj->AddEnergyBar();
		GameCallEx("OnCreationRuleNoFF", obj);	
	}
	
	for (var obj in FindObjects(Find_ID(InventorsLab)))
	{
		obj->CreateEffect(FxMinionSpawner,1,1);
	}
	
	g_goalmoba_init = true;
}

func InitStuff()
{
	CreateObjectAbove(WoodenBridge, 516, 2012);
	CreateObjectAbove(WoodenBridge, 1077, 1661);
	CreateObjectAbove(WoodenBridge, 1933, 1404);
	CreateObjectAbove(WoodenBridge, 1779, 1668);
	CreateObjectAbove(WoodenBridge, 1493, 1780);
	CreateObjectAbove(WoodenBridge, 987, 2379);
	CreateObjectAbove(WoodenBridge, 1532, 2772);
	CreateObjectAbove(WoodenBridge, 1947, 3028);
	CreateObjectAbove(WoodenBridge, 1333, 2860);
	CreateObjectAbove(WoodenBridge, 797, 2540);
	CreateObjectAbove(WoodenBridge, 4620, 1668);
	CreateObjectAbove(WoodenBridge, 4907, 1780);
	CreateObjectAbove(WoodenBridge, 5324, 1660);
	CreateObjectAbove(WoodenBridge, 5884, 2012);
	CreateObjectAbove(WoodenBridge, 4868, 2772);
	CreateObjectAbove(WoodenBridge, 5067, 2860);
	CreateObjectAbove(WoodenBridge, 4453, 3028);
	CreateObjectAbove(WoodenBridge, 5412, 2380);
	CreateObjectAbove(WoodenBridge, 4467, 1404);
	CreateObjectAbove(WoodenBridge, 5602, 2539);
	
	CreateObjectAbove(WoodenBridge, 2860, 1074);
	CreateObjectAbove(WoodenBridge, 3540, 1074);
	
	for (var bridge in FindObjects(Find_ID(WoodenBridge)))
	{
		bridge->SetClrModulation(0xff00ffff);
		bridge->SetHalfVehicleSolidMask(true);
		bridge->SetCategory(C4D_Vehicle);
		bridge->MakeInvincible(true);
	}
	
	CreateObject(JumpPad, 3684, 2287);
	CreateObject(JumpPad, 2716, 2287);
	CreateObject(JumpPad, 2514, 2287);
	CreateObject(JumpPad, 3885, 2287);
	
	var jp = CreateObject(JumpPad, 2860, 2000);
	jp.Strength = 194;
	jp = CreateObject(JumpPad, 3540, 2000);
	jp.Strength = 194;
	jp = CreateObject(JumpPad, 2427, 2887);
	jp.Strength = 150;
	jp = CreateObject(JumpPad, 3971, 2887);
	jp.Strength = 150;
	
	g_deftw1  = CreateObjectAbove(DefenseTower, 986, 1646);
	g_deftw2  = CreateObjectAbove(DefenseTower, 1804, 1390);
	g_deftw3  = CreateObjectAbove(DefenseTower, 2662, 1063);
	g_deftw4  = CreateObjectAbove(DefenseTower, 3737, 1063);
	g_deftw5  = CreateObjectAbove(DefenseTower, 4594, 1390);
	g_deftw6  = CreateObjectAbove(DefenseTower, 5416, 1647);
	g_deftw7  = CreateObjectAbove(DefenseTower, 931, 2366);
	g_deftw8  = CreateObjectAbove(DefenseTower, 2003, 3014);
	g_deftw9  = CreateObjectAbove(DefenseTower, 2898, 2887);
	g_deftw10 = CreateObjectAbove(DefenseTower, 3501, 2887);
	g_deftw11 = CreateObjectAbove(DefenseTower, 4401, 3015);
	g_deftw12 = CreateObjectAbove(DefenseTower, 5472, 2366);
	
	g_deftw2->SetParentTower(g_deftw3);
	g_deftw1->SetParentTower(g_deftw2);
	g_deftw7->SetParentTower(g_deftw8);
	g_deftw8->SetParentTower(g_deftw2);
	g_deftw5->SetParentTower(g_deftw4);
	g_deftw6->SetParentTower(g_deftw5);
	g_deftw11->SetParentTower(g_deftw10);
	g_deftw12->SetParentTower(g_deftw11);
	
	g_invll = CreateObjectAbove(InventorsLab, 236, 2000);
	g_invlr = CreateObjectAbove(InventorsLab, 6159, 1998);
	
	g_invll->SetTopTower(g_deftw1);
	g_invll->SetBotTower(g_deftw7);
	g_invlr->SetTopTower(g_deftw6);
	g_invlr->SetBotTower(g_deftw12);
}

public func GiveRandomAttackTarget(object attacker)
{
	// Minions and Players first
	var target = attacker->FindObject(Find_OCF(OCF_Alive), Find_Hostile(attacker->GetController()), Find_Distance(200), Sort_Distance());
	
	var towertarget;
	
	// Attack Towers and Bases then
	if (attacker->GetOwner() == g_plrid_minions_left)
	{
		if (attacker.Lane == "Top")
		{
			towertarget =  g_deftw4 ?? g_deftw5 ?? g_deftw6 ?? g_invlr;
		}
		
		if (attacker.Lane == "Bot")
		{
			towertarget =  g_deftw10 ?? g_deftw11 ?? g_deftw12 ?? g_invlr;
		}
	}
	
	if (attacker->GetOwner() == g_plrid_minions_right)
	{
		if (attacker.Lane == "Top")
		{
			towertarget =  g_deftw3 ?? g_deftw2 ?? g_deftw1 ?? g_invll;
		}
		
		if (attacker.Lane == "Bot")
		{
			towertarget =  g_deftw9 ?? g_deftw8 ?? g_deftw7 ?? g_invll;
		}
	}
	
	// Towerattackers prefer towers, if near enough
	//if (attacker.isTowerattacker && ObjectDistance(towertarget, attacker) < 200)
		//return towertarget;
	
	if (target)
		return target;
	
	if (towertarget)
		return towertarget;
	
	return;
}

func SpawnPlayer(int plr, object clonk)
{	
	clonk->CreateContents(Sword);
	clonk->SelectChampion();
	
	var invl = FindObject(Find_ID(InventorsLab),Find_Team(GetPlayerTeam(clonk->GetOwner())));
	
	var relaunch = CreateObject(RelaunchContainer, invl->GetX(), invl->GetY(), clonk->GetOwner());
	relaunch->StartRelaunch(clonk);
	
	GameCallEx("OnCreationRuleNoFF", clonk);
}
















