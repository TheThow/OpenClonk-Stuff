/**
	Enraged
	Defense round for Knueppeln.

	@author Maikel
*/

static init_defenders;


protected func Initialize()
{
	// Defense goal.
	CreateObject(Goal_Defense);
	
	// Rules.
	ActivateMedalRule();
	CreateObject(Rule_NoFriendlyFire);
	CreateObject(Rule_Items)->Set(200);
	CreateObject(Rule_FastMana);
	CreateObject(Env_MedalRewards);
	
	// Statistics.
	InitStatistics();
	return;
}


/*-- Player Control --*/

protected func InitializePlayer(int plr)
{
	if (GetPlayerType(plr) == C4PT_Script)
		return;
	
	// Move players to defenders team.
	if (GetPlayerTeam(plr) != 1)
		SetPlayerTeam(plr, 1);
	
	// Spawn player.
	SpawnPlayer(plr);
	
	// Set zoom ranges.
	SetPlayerZoomByViewRange(plr, 1200, nil, PLRZOOM_LimitMax);
	SetPlayerZoomByViewRange(plr, 500, nil, PLRZOOM_Direct);
	
	// Give base and set wealth.
	if (!init_defenders)
	{
		for (var obj in FindObjects(Find_Or(Find_ID(InventorsLab), Find_ID(DefenseTower))))
		{
			obj->SetOwner(plr);
			obj->SetCategory(C4D_Living);
			obj->SetAlive(true);
			obj->AddVertex(0, obj->GetBottom() + 4);
			obj.MaxEnergy = 400000;
			obj->DoEnergy(obj.MaxEnergy / 1000);
			obj->AddEnergyBar();
			GameCallEx("OnCreationRuleNoFF", obj);	
		}
		init_defenders = true;
	}
	return;
}

public func OnClonkDeath(object clonk)
{
	if (!FindObject(Find_Or(Find_ID(InventorsLab), Find_ID(DefenseTower))))
		return;	
	var plr = clonk->GetOwner();
	if (GetPlayerType(plr) == C4PT_Script)
		return;
	var new_clonk = CreateObjectAbove(Clonk, 0, 0, plr);
	new_clonk->MakeCrewMember(plr);
	new_clonk.MaxMagic = clonk.MaxMagic;
	SetCursor(plr, new_clonk);
	SpawnPlayer(plr);
	return;
}

public func SpawnPlayer(int plr)
{
	var clonk = GetCrew(plr);
	if (!clonk)
	{
		Log("WARNING: player %d has no clonk.", plr);
		LogCallStack();
		return;
	}
	clonk->SetPosition(70, 406);	
	clonk->CreateContents(Sword);
	clonk->SetMagicEnergy(50);
	var relaunch = CreateObjectAbove(RelaunchContainer, 70, 406, clonk->GetOwner());
	relaunch->SetRelaunchTime(10);
	relaunch->StartRelaunch(clonk);	
	ScheduleCall(clonk, "SelectChampion", 1, 0);
	return;
}

public func OnClonkLeftRelaunch(object clonk)
{
	clonk->Exit();
	clonk->SetPosition(70, 406);
	return;
}


/*-- Waves Control --*/

public func GetAttackWave(int nr)
{
	// The round starts with a short phase to prepare.
	if (nr == 1) 
		return new DefenseEnemy.BreakWave { Duration = 15 };
	
	// Attack positions.
	var pos_rockets = {X = LandscapeWidth() - 10, Y = 10};
	var pos_below = {X = LandscapeWidth() - 10, Y = 504, Exact = true};
	var pos_middle = {X = LandscapeWidth() - 10, Y = 416, Exact = true};
	var pos_above = {X = LandscapeWidth() - 10, Y = 200, Exact = true};
	

	// Automatically build waves that become stronger.
	var wave = new DefenseEnemy.DefaultWave
	{
		Name = "$MsgWave$",
		// Waves last shorter as the number increases.
		Duration = nil,
		Bounty = 50,
		Score = 50,
		Enemies = []
	};
	
	// Add enemy: boom attack.
	PushBack(wave.Enemies, new DefenseEnemy.BoomAttack {
		Amount = nr - 1,
		Speed = 80 + (nr - 2) * 20,
		Position = pos_rockets
	});
	
	// Add enemy: champs.
	PushBack(wave.Enemies, new DefenseEnemy.Champ {
		Amount = (nr + 1) / 3,
		Energy = 10 + nr * 5,
		Magic = 100 + 10 * nr,
		Position = pos_middle
	});
	PushBack(wave.Enemies, new DefenseEnemy.Champ {
		Amount = (nr + 0) / 3,
		Energy = 10 + nr * 5,
		Magic = 100 + 10 * nr,
		Position = pos_middle
	});
	PushBack(wave.Enemies, new DefenseEnemy.Champ {
		Amount = (nr - 1) / 3,
		Energy = 10 + nr * 5,
		Magic = 100 + 10 * nr,
		Position = pos_above
	});
	return wave;
}

// The attackers should go for all things alive.
public func GiveRandomAttackTarget(object attacker)
{
	var target = attacker->FindObject(Find_OCF(OCF_Alive), Find_Hostile(attacker->GetController()), Sort_Distance());
	if (target)
		return target;
	return;
}