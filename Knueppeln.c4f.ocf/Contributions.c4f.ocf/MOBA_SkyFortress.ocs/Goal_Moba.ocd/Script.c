/*

	Moba Gamemode for 2 Teams
	
	@author: K-Pone

*/

#include Library_Goal

static g_goalmoba_init;
local g_goalmoba_base_left;
local g_goalmoba_base_right;

func Initialize()
{
	
}

public func InitializeScriptPlayer(int plr, int team)
{
	if (team == 1)
		g_plrid_minions_left = plr;
			
	if (team == 2)
		g_plrid_minions_right = plr;
	
	if (g_plrid_minions_left && g_plrid_minions_right)
		GameCall("InitAIStuff");
}

func IsFulfilled()
{
	if (!g_goalmoba_init) return false;
	if (g_goalmoba_base_left && g_goalmoba_base_right) return false;
	
	if (!g_goalmoba_base_left)
	{
		// Left team lost
		for(var i = 0; i < GetPlayerCount(); i++)
		{
			if (GetPlayerTeam(GetPlayerByIndex(i)) == 1)
				EliminatePlayer(GetPlayerByIndex(i));
		}
	}
	
	if (!g_goalmoba_base_right)
	{
		// Left team lost
		for(var i = 0; i < GetPlayerCount(); i++)
		{
			if (GetPlayerTeam(GetPlayerByIndex(i)) == 2)
				EliminatePlayer(GetPlayerByIndex(i));
		}
	}
	
	return true;
}

func SetLeftBase(object base)
{
	g_goalmoba_base_left = base;
}

func SetRightBase(object base)
{
	g_goalmoba_base_right = base;
}

public func Activate(int byplr)
{
	MessageWindow("MOBA YEAH!!!!", byplr);
}

func RelaunchPlayer(int plr, int killer)
{
	if (plr == g_plrid_minions_left || plr == g_plrid_minions_right)
	{
		// called for minions
	}
	else
	{
		// called for players
		var clonk = CreateObjectAbove(Clonk, 0, 0, plr);
		clonk->MakeCrewMember(plr);
		SetCursor(plr, clonk);
		GameCall("SpawnPlayer", plr, clonk);
	}
}

