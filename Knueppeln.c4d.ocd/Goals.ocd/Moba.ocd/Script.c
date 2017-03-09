/**
	Moba Goal
	Moba game mode for 2 teams. Handles the goal and minion spawning, minions are spawned
	at the bases of the two teams and then choose their paths.
	
	@author K-Pone, Maikel
*/

#include Library_Goal


local base_left;
local base_right;
local color_left;
local color_right;
local minion_plr_left;
local minion_plr_right;
local nr_lanes;
local minions_killed;

public func Initialize()
{
	nr_lanes = 2;
	color_left = RGB(225, 0, 0);
	color_right = RGB(64, 64, 255);
	// Create script players for controlling minions.
	CreateScriptPlayer("$MinionTeamLeft$", color_left, 1, CSPF_FixedAttributes | CSPF_NoScenarioInit | CSPF_NoEliminationCheck | CSPF_Invisible, Goal_Moba);
	CreateScriptPlayer("$MinionTeamRight$", color_right, 2, CSPF_FixedAttributes | CSPF_NoScenarioInit | CSPF_NoEliminationCheck | CSPF_Invisible, Goal_Moba);
	// Initialize scoreboard.
	minions_killed = [];
	Scoreboard->SetTitle("$MsgScoreboard$");
	Scoreboard->Init([
		{key = "minions_killed", title = Boompack, sorted = true, desc = true, default = "0", priority = 100}
	]);
	return _inherited(...);
}

public func InitializeScriptPlayer(int plr, int team)
{
	// Forward to goal object.
	if (this == Goal_Moba)
	{
		var goal = FindObject(Find_ID(Goal_Moba));
		if (goal)
			goal->InitializeScriptPlayer(plr, team);
		return;
	}	
	
	// Store minion players.
	if (team == 1)
	{
		minion_plr_left = plr;
		InitBase(base_left, minion_plr_left, 1, color_left);
	}
	if (team == 2)
	{
		minion_plr_right = plr;
		InitBase(base_right, minion_plr_right, 2, color_right);
	}
	// Do callback to scenario if both minion players have been initialized.
	if (minion_plr_left && minion_plr_right)
		GameCall("InitAIObjects", minion_plr_left, minion_plr_right);
	return;	
}

public func IsFulfilled()
{
	if (!base_left && !base_right)
		return false;
	if (base_left && base_right)
		return false;
	
	if (!base_left)
		EliminateTeam(1);
	else if (!base_right)
		EliminateTeam(2);
	return true;
}

public func EliminateTeam(int team)
{
	for (var plr in GetPlayers(nil, team))
	{
		// Remove all minions and then eliminate player.
		RemoveAll(Find_Property("IsMobaMinion"), Find_Owner(plr));
		EliminatePlayer(plr);
	}
	return;
}


/*-- Bases --*/

public func SetLeftBase(object base)
{
	base_left = base;
	base_left.IsMinionTarget = true;
	InitBase(base_left, minion_plr_left, 1, color_left);
	return;
}

public func SetRightBase(object base)
{
	base_right = base;
	base_right.IsMinionTarget = true;
	InitBase(base_right, minion_plr_right, 2, color_right);
	return;
}

public func InitBase(object base, int plr, int team, int color)
{
	if (!base || plr == nil || team == nil)
		return;
	base->SetOwner(plr);
	base->CreateEffect(FxMinionSpawner, 100, 1, team, color, nr_lanes);
	return;
}


/*-- Player Control --*/

public func InitializePlayer(int plr)
{
	// The enemy script player is initialized in the function below.
	if (GetPlayerType(plr) == C4PT_Script)
		return;
	// Init the normal players
	var plrid = GetPlayerID(plr);
	// Initialize scoreboard.
	Scoreboard->NewPlayerEntry(plr);
	minions_killed[plrid] = 0;
	Scoreboard->SetPlayerData(plr, "minions_killed", minions_killed[plrid]);
	return;
}

public func RelaunchPlayer(int plr, int killer)
{
	if (plr == minion_plr_left || plr == minion_plr_right)
	{
		// Called for minions.
	}
	else
	{
		// Called for players.
		var clonk = CreateObjectAbove(Clonk, 0, 0, plr);
		clonk->MakeCrewMember(plr);
		SetCursor(plr, clonk);
		GameCall("SpawnPlayer", plr, clonk);
	}
}


/*-- Minion Spawning --*/


public func SetLaneCount(int lanes)
{
	nr_lanes = Max(1, lanes);
	var fx, index = 0;
	while (fx = GetEffect("FxMinionSpawner", this, index++))
		fx.nr_lanes = nr_lanes;
	return;
}

local FxMinionSpawner = new Effect
{	
	Construction = func(int team, int color, int nr_lanes)
	{
		this.team = team;
		this.color = color;
		this.nr_lanes = nr_lanes;
		this.lane = 0;
		// Minion rate.
		this.Interval = 10;
		this.minion_int = 2;
		this.minion_group_size = 5;
		this.group_int = 30;
		// First timer call.
		Timer(0);
		return FX_OK;
	},
	
	Timer = func(int time)
	{
		if ((time / this.Interval) % this.minion_int == 0 && ((time / this.Interval) % this.group_int) < this.minion_group_size * this.minion_int)
			SpawnMinion(Sword, this.lane);
		if (((time + this.Interval) / this.Interval) % this.group_int == 0)
		{
			this.lane++;
			this.lane = this.lane % this.nr_lanes;
		}
		
		return FX_OK;
	},
	
	SpawnMinion = func(id weapon, int lane)
	{
		var minion = Target->CreateObjectAbove(Clonk, 0, Target->GetBottom(), Target->GetOwner());
		minion->CreateContents(weapon);
		minion.IsMobaMinion = true;
		minion->SetClrModulation(this.color);		
		minion.Lane = lane;
		minion.Team = this.team;
		MobaMinionAI->AddAI(minion);
		GameCallEx("OnCreationRuleNoFF", minion);
		return;
	}
};


/*-- Scoreboard --*/

public func OnClonkDeath(object clonk, int killer)
{
	if (clonk.IsMobaMinion && GetPlayerType(killer) == C4PT_User)
	{
		var plrid = GetPlayerID(killer);
		minions_killed[plrid]++;
		
		Scoreboard->SetPlayerData(killer, "minions_killed", minions_killed[plrid]);
	}
	return;
}


/*-- Description --*/

public func GetDescription(int plr)
{
	return this.Description;
}


/*-- Properties --*/

local Name = "$Name$";
local Description = "$Description$";
local Visibility = VIS_Editor;
local EditorPlacementLimit = 1;