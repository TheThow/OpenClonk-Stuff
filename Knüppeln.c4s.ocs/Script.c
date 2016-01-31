/**
	Knüppeln.c4s

	@author KKenny / Thow
*/



func Initialize()
{
	CreateObject(Goal_LastManStanding);
}


func InitializePlayer(int plr, int iX, int iY, object pBase, int iTeam)
{
	JoinPlayer(plr);
	return;
}

func JoinPlayer(int plr, prev_clonk)
{
	var clonk = GetCrew(plr);
	clonk->CreateContents(Sword);
	clonk->SetMagicEnergy(50);
	clonk->ChooseMenu();
}

// Gamecall from LastManStanding goal, on respawning.
protected func OnPlayerRelaunch(int plr)
{
	var clonk = GetCrew(plr);
	var relaunch = CreateObjectAbove(RelaunchContainer, LandscapeWidth() / 2, LandscapeHeight() / 2, clonk->GetOwner());
	relaunch->StartRelaunch(clonk);
	
	JoinPlayer(plr);
	
	return;
}

global func GetRandomSpawn()
{
	var spawns = [[50,155],[150,200],[310,200],[610,200],[750,200],[915,155]];
	var rand = Random(GetLength(spawns));
	return spawns[rand];
}

func OnClonkLeftRelaunch(object clonk)
{
	var pos = GetRandomSpawn();
	clonk->SetPosition(pos[0],pos[1]);
	return;
}

func KillsToRelaunch() { return 0; }
