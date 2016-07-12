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
		
		Scoreboard->SetData(team_id, "score", score, score);
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
	InitTeamExclusiveChamps(GetStartupTeamCount());
	InitScoreboard();
	
	team_score = CreateArray(GetStartupTeamCount());
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


global func InitScoreboard()
{
	Scoreboard->Init(
	[
	 	{key = "team", title = "", sorted = true, priority = 100, default = ""},
	 	{key = "score", title = "Score", priority = 200, default = "0"}]
	);
	
	for(var i = 0; i < GetStartupTeamCount(); i++)
	{
		var team_id = GetTeamByIndex(i);
		Scoreboard->NewEntry(team_id, GetTaggedTeamName(team_id));
		Scoreboard->SetData(team_id, "team", "", team_id);
		Scoreboard->SetData(team_id, "score", 0, 0);
	}
}