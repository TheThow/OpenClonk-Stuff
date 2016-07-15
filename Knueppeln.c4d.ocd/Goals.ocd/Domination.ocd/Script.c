/**
	BombRun
	

	@author 
*/

#include Goal_Melee

local Name = "$Name$";
local Description = "$Description$";
local Plane = 100;

local team_score;
local victory;

local team_kills;
local team_deaths;

local player_kills;
local player_deaths;

local scoreboard_initialized = false;

local CheckFx = new Effect {

	Timer = func() {
		
		
		if (Target.victory)
			return -1;
		
		var c = false;
	
		for (var o in FindObjects(Find_ID(DominationPoint)))
		{
			var team = o->GetTeam();
			
			if (team)
			{
				var team_id = GetTeamByIndex(team-1);
				Target.team_score[team_id - 1]++;
				c = true;
			}
		}
		
		if(c)
		{
			Target->UpdateScoreboard();
			Target->CheckVictory();
		}
	
	}

};

func UpdateScoreboard()
{
	for(var i = 0; i < GetLength(team_score); i++)
	{
		var team_id = GetTeamByIndex(i);
		
		var score = team_score[team_id - 1];
		if(score == nil)
			score = 0;
		
		Scoreboard->SetData(CalculateTeamID(team_id), "score", score, score);
	}
}

func CheckVictory()
{
	if(victory)
		return;

	for (var i = 0; i < GetLength(team_score); i++)
	{
		var team_id = GetTeamByIndex(i);
		if (team_score[team_id - 1] >= GameCall("ScoreToWin"))
		{
			for(var p = 0; p < GetPlayerCount(); p++)
			{
				if(GetPlayerTeam(GetPlayerByIndex(p)) != team_id)
				{
					EliminatePlayer(GetPlayerByIndex(p));
				}
			}
			
			ScheduleCall(nil, "GameOver", 200);
			Sound("UI::Trumpet", true, 100);
			victory = true;
			break;
		}
	}
}

func Initialize()
{
	team_deaths = [];
	team_kills = [];
	player_deaths = [];
	player_kills = [];
	
	CreateEffect(CheckFx, 1, 40);
	victory = false;
}


func UseTeamExclusiveChampions() { return true; }

func NoRelaunchReleaseOnSelection() { return true; }


protected func RelaunchPlayer(int plr, int killer)
{
	_inherited(plr, killer, ...);
				
	var clonk = CreateObjectAbove(Clonk, 0, 0, plr);
	clonk->MakeCrewMember(plr);
	SetCursor(plr, clonk);
	// Scenario script callback.
	GameCall("OnPlayerRelaunch", plr, true);
	// Show scoreboard for a while.
	return; // _inherited(plr, killer, ...);
}

public func CalculateTeamID(int team_id)
{
	return 1000 + 100 * team_id;
}

public func OnClonkDeath(object clonk, int killed_by)
{
	var plr = clonk->GetOwner();
	if (!GetPlayerName(plr)) return;
	
	if (Hostile(killed_by, plr))
	{
		// update kills
		var enemy_team = GetPlayerTeam(killed_by);
		Scoreboard->SetPlayerData(killed_by, "kills", ++player_kills[killed_by]);
		Scoreboard->SetData(CalculateTeamID(enemy_team), "kills", ++team_kills[enemy_team]);
	}

	// updates deaths
	var team = GetPlayerTeam(plr);
	Scoreboard->SetPlayerData(plr, "deaths", ++player_deaths[plr]);
	Scoreboard->SetData(CalculateTeamID(team), "deaths", ++team_deaths[team]);
	
	return _inherited(clonk, plr, killed_by, ...);
}

public func InitializePlayer(int newplr, int x, int y, object base, int team)
{
	InitScoreboard();
	
	player_kills[newplr] = 0;
	player_deaths[newplr] = 0;
	
	Scoreboard->NewPlayerEntry(newplr);
	Scoreboard->SetPlayerData(newplr, "team", "", CalculateTeamID(team) + 1);
	
	return _inherited(newplr, x, y, base, team, ...);
}

func RemovePlayer(plr, team)
{
	if (!victory)
		Scoreboard->SetPlayerData(plr, "deaths", -1);
	return _inherited(plr, team, ...);
}

public func InitScoreboard()
{
	if (scoreboard_initialized) return false;
	scoreboard_initialized = true;

	InitTeamExclusiveChamps(GetStartupTeamCount());
	team_score = CreateArray(GetStartupTeamCount());
	
	Scoreboard->Init(
	[
	 	{key = "team", title = "", sorted = true, priority = 200, default = ""},
	 	{key = "score", title = "Score", priority = 100, default = 0},
	 	{key = "kills", title = Sword, sorted = true, desc = true, priority = 75, default = 0},
		{key = "deaths", title = Clonk, sorted = true, desc = false, priority = 50, default = 0, conditional = Scoreboard_Death.ScoreboardCondition}
	]
	);
	
	for(var i = 0; i < GetStartupTeamCount(); i++)
	{
		var team_id = GetTeamByIndex(i);
		var team_name = Format("Team %s", GetTaggedTeamName(team_id));
		team_id = CalculateTeamID(team_id);
		Scoreboard->NewEntry(team_id, team_name);
		Scoreboard->SetData(team_id, "team", "", team_id);
		Scoreboard->SetData(team_id, "score", 0, 0);
	}
}