/**
	Sky Fortress / Himmelsfestung
	
	@author K-Pone
*/


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


public func Initialize()
{
	// Rules.
	CreateObject(Rule_NoFriendlyFire);
	
	InitObjects();
	
	// Goal.
	var goal = CreateObject(Goal_Moba);
	goal->SetLeftBase(g_invll);
	goal->SetRightBase(g_invlr);

	// Gather statistics.
	InitStatistics(STATS_Type_All);	
}

public func OnGoalsFulfilled()
{
	Sound("RoundEnd", true);
	ScheduleCall(nil, "GameOver", 80, 0);
	return true;
}

public func InitializePlayer(int plr, int x, int y, int base, team, extradata)
{	
	var crew = GetCrew(plr);
	var relaunch = CreateObject(RelaunchContainer, LandscapeWidth() / 2, LandscapeHeight() / 2);
	relaunch->StartRelaunch(crew);
	crew->CreateContents(Sword);
	
	ScheduleCall(nil, "PostPlayerInit", 15, 0, plr, team, crew, relaunch);
	return;
}

global func PostPlayerInit(int plr, int team, object crew, object relaunch)
{	
	var invl = FindObject(Find_ID(InventorsLab), Find_Team(team));
	
	crew->~SelectChampion();
	ScheduleCall(crew, "SelectChampion", 15, 0);
	
	relaunch->SetPosition(invl->GetX(), invl->GetY());
	return;
} 

public func InitAIObjects(int left_minion_plr, int right_minion_plr)
{	
	// AI Players created. Assign Objects to AI players. Useful when no human players joined the enemy team.
	for (var obj in FindObjects(Find_Or(Find_ID(InventorsLab), Find_ID(DefenseTower))))
	{
		if (obj->GetX() < LandscapeWidth() / 2)
			obj->SetOwner(left_minion_plr);
		else
			obj->SetOwner(right_minion_plr);
		obj->SetCategory(C4D_Living);
		obj->SetAlive(true);
		obj->AddVertex(0, obj->GetBottom() + 4);
		obj.MaxEnergy = 400000;
		obj->DoEnergy(obj.MaxEnergy / 1000);
		obj->AddEnergyBar();
		GameCallEx("OnCreationRuleNoFF", obj);	
	}
	return;
}

public func InitObjects()
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
	// Minions and champs first.
	var target = attacker->FindObject(Find_OCF(OCF_Alive), Find_Hostile(attacker->GetController()), Find_Distance(200), Sort_Distance());
	if (target)
		return target;
		
	var towertarget;
	// Attack Towers and Bases then
	if (attacker.Team == 1)
	{
		if (attacker.Lane == 0)
		{
			towertarget = g_deftw4 ?? g_deftw5 ?? g_deftw6 ?? g_invlr;
		}
		
		if (attacker.Lane == 1)
		{
			towertarget = g_deftw10 ?? g_deftw11 ?? g_deftw12 ?? g_invlr;
		}
	}
	
	if (attacker.Team == 2)
	{
		if (attacker.Lane == 0)
		{
			towertarget = g_deftw3 ?? g_deftw2 ?? g_deftw1 ?? g_invll;
		}
		
		if (attacker.Lane == 1)
		{
			towertarget = g_deftw9 ?? g_deftw8 ?? g_deftw7 ?? g_invll;
		}
	}
	return towertarget;
}

public func SpawnPlayer(int plr, object clonk)
{	
	clonk->CreateContents(Sword);
	clonk->SelectChampion();
	var invl = FindObject(Find_ID(InventorsLab),Find_Team(GetPlayerTeam(clonk->GetOwner())));
	var relaunch = CreateObject(RelaunchContainer, invl->GetX(), invl->GetY(), clonk->GetOwner());
	relaunch->StartRelaunch(clonk);
}