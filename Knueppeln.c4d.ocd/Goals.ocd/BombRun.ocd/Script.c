/**
	BombRun
	

	@author 
*/

#include Goal_Melee

local Name = "$Name$";
local Description = "$Description$";
local Plane = 100;

local ball;
local ballID = Wipf;

local maxDist = 50;
local resetTime = 200;

local team_score;

local leftgoal;
local rightgoal;
local leftshield;
local rightshield;
local shieldradius;

local rightshield_params;
local leftshield_params;

local pause = false;

func Initialize()
{
	AddEffect("DefMana", nil, 1, 4, nil);

	InitTeamExclusiveChamps(2);
	InitScoreboard();
	
	team_score = CreateArray(2);

	var fx = AddEffect("CheckWipf", this, 1, 5, this);
	fx.counter = resetTime;
	
	var pos = GameCall("LeftGoalPos");
	leftgoal = CreateObject(Dummy, pos[0], pos[1], -1);
	leftgoal.Visibility = VIS_All;
	var fxl = AddEffect("GoalCheck", leftgoal, 1, 1, this, nil, 1, 2);
	fxl.teamid = 1;
	fxl.enemy = 2;
	leftshield_params = GameCall("LeftShieldParams");
	leftshield = CreateObject(PortalWall,leftshield_params[0], leftshield_params[1],-1);
	
	pos = GameCall("RightGoalPos");
	rightgoal = CreateObject(Dummy, pos[0], pos[1], -1);
	rightgoal.Visibility = VIS_All;
	var fxr = AddEffect("GoalCheck", rightgoal, 1, 1, this, nil, 2, 1);
	fxr.teamid = 2;
	fxr.enemy = 1;
	rightshield_params = GameCall("RightShieldParams");
	rightshield = CreateObject(PortalWall,rightshield_params[0],rightshield_params[1],-1);
	
	SpawnBall();
	
	ScheduleCall(this, "Set", 80);
}


global func FxDefManaTimer()
{
	for(var o in FindObjects(Find_ID(Clonk)))
	{
		var plr = o->GetOwner();
		if(GetPlayerTeam(plr) == 1 && o->GetX() < LandscapeWidth()/2)
			o->DoMagicEnergy(1);
			
		if(GetPlayerTeam(plr) == 2 && o->GetX() > LandscapeWidth()/2)
			o->DoMagicEnergy(1);
	}
}

func UseTeamExclusiveChampions() { return true; }

func NoRelaunchReleaseOnSelection() { return true; }

func FxGoalCheckStart(object target, proplist effect, int temporary, var1, var2)
{
	var clr = GetTeamColor(var1);
	var rgba = SplitRGBaValue(clr);

	effect.particles = {
		R = rgba.R,
		G = rgba.G,
		B = rgba.B,
		Size = 5,
		Alpha = PV_Linear(255, 0)
	};
}

func FxGoalCheckTimer(object target, proplist effect, int time)
{

	effect.angle += 3;
	target->CreateParticle("Flash", Cos(effect.angle, 15), Sin(effect.angle, 30), 0, 0, 50, effect.particles, 1);
	target->CreateParticle("Flash", Cos(effect.angle - 180, 15), Sin(effect.angle - 180, 30), 0, 0, 50, effect.particles, 1);
	
	var b = (target->FindObject(Find_ID(ballID), Find_InRect(-12, -30, 24, 60)));
	
	if(b)
	{
		Log("Team %s scored!", GetTaggedTeamName(effect.enemy));
		team_score[effect.enemy - 1]++;
		b->RemoveObject();
		ScoreEffect(target);
		pause = true;
		
		Scoreboard->SetData(effect.enemy, "score", team_score[effect.enemy - 1]);
		
		DoScoreboardShow(1);
		Schedule(nil, Format("DoScoreboardShow(-1)"), 36 * 10);
		
		
		if (team_score[effect.enemy - 1] == GameCall("ScoreToWin"))
		{
			for(var i = 0; i < GetPlayerCount(); i++)
			{
				if(GetPlayerTeam(GetPlayerByIndex(i)) == effect.teamid)
				{
					EliminatePlayer(GetPlayerByIndex(i));
				}
			}
			
			ScheduleCall(nil, "GameOver", 160);
			Sound("UI::Trumpet", true, 100);
		}
		else
		{
			ResetTeamExclusiveChamps();
			for(var i = 0; i < GetPlayerCount(); i++)
			{
				ScheduleCall(this, "ResetPlayer", 120, 0, GetPlayerByIndex(i));
				ScheduleCall(this, "Set", 200);
			}
		}
	}
}

func ResetPlayer(plr)
{
	GetCrew(plr).ChampType->CleanUp(GetCrew(plr));
	GetCrew(plr).ChampType = Man;
	GameCall("SpawnPlayer", plr, 10);
	ScheduleCall(GetCrew(plr), "SelectChampion", 15, 0);
	GetCrew(plr)->DoEnergy(100);
	
}

func Set()
{
	leftshield->CreateWall(1, leftshield_params[2], leftshield_params[3], 75 + (25 * GetPlayerCount()));
	rightshield->CreateWall(2, rightshield_params[2], rightshield_params[3], 75 + (25 * GetPlayerCount()));
	pause = false;
}

func ScoreEffect(target)
{
	Sound("Animals::Wipf::Whine", true, 100);
	Sound("Structures::SawmillRipcut", true, 100);
	Sound("UI::Ding", true, 100);
	
	AddEffect("Blood", target, 1, 1, this);
}

func FxBloodTimer(object target, proplist effect, int time)
{
	if (time > 50)
		return -1;
	
	var props = {
		Size = PV_Linear(PV_Random(2, 4), 0),
		CollisionVertex = 500,
		OnCollision = PC_Bounce(200),
		ForceY = PV_Gravity(300),
		R = 128,
		G = 0,
		B = 0
	};
	
	var t = 0;
	var ta = 0;
	if(target == rightgoal)
	{
		t = 80;
		ta = 180;
	}
	target->CreateParticle("Flash", 0, PV_Random(-10,10), PV_Random(-60 + t, -20 + t), PV_Random(-60, 30), 140, props, 5);
	
	if(time % 10 == 0)
	{
		target->CastObjects(Flesh, 1, 40, 0, RandomX(-10,10), -90 +ta, 50);
	}
}

func FxCheckWipfTimer(object target, proplist effect, int time)
{
	if(!ball && !pause)
	{
		SpawnBall();
		CustomMessage("$Reset$");
	}
	
	if(!ball)
		return;
	
	var pos = GameCall("BallPos");
	var dist = Distance(ball->GetX(), ball->GetY(), pos[0], pos[1]);
	
	if (dist > maxDist && !FindObject(Find_ID(Clonk), Find_Distance(maxDist, ball->GetX(), ball->GetY())))
	{
		effect.counter -= 5;
		ball->Message("Reset in: %d", effect.counter / 40 + 1);
		
		if (effect.counter <= 0)
		{
			ball->SetPosition(pos[0], pos[1]);
			CustomMessage("$Reset$");
		}
	}
	
	if (dist < maxDist || FindObject(Find_ID(Clonk), Find_Distance(maxDist, ball->GetX(), ball->GetY())))
	{
		effect.counter = resetTime;
		ball->Message("");
	}
}

func SpawnBall()
{
	var pos = GameCall("BallPos");
	ball = CreateObject(ballID, pos[0], pos[1]);
}

protected func RelaunchPlayer(int plr, int killer)
{
	_inherited(plr, killer, ...);
				
	var clonk = CreateObjectAbove(Clonk, 0, 0, plr);
	clonk->MakeCrewMember(plr);
	SetCursor(plr, clonk);
	// Scenario script callback.
	GameCall("OnPlayerRelaunch", plr, true);
	// Show scoreboard for a while.
	DoScoreboardShow(1, plr + 1);
	Schedule(this,Format("DoScoreboardShow(-1, %d)", plr + 1), 36 * 10);
	return; // _inherited(plr, killer, ...);
}


global func InitScoreboard()
{
	Scoreboard->Init(
	[
	 	{key = "team", title = "", sorted = true, priority = 200, default = ""},
	 	{key = "score", title = "Score", priority = 100, default = ""}]
	);
	
	for(var i = 1; i <= 2; i++)
	{
		var team_id = i;
		Scoreboard->NewEntry(team_id, GetTaggedTeamName(i));
		Scoreboard->SetData(team_id, "team", "", team_id);
		Scoreboard->SetData(team_id, "score", 0);
	}
}