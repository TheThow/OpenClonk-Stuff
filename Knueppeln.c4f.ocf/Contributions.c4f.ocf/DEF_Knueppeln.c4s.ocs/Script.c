/**
	Knüppeln Defense
	Defense round for Knueppeln.

	@author K-Pone, Maikel
*/

static init_defenders;


protected func Initialize()
{
	// Defense goal.
	CreateObject(Goal_Defense);
	
	// Rules.
	CreateObject(Rule_NoFriendlyFire);
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
	SetPlayerZoomByViewRange(plr, 800, nil, PLRZOOM_LimitMax);
	SetPlayerZoomByViewRange(plr, 450, nil, PLRZOOM_Direct);
	
	// Give base and set wealth.
	if (!init_defenders)
	{
		for (var obj in FindObjects(Find_Or(Find_ID(InventorsLab), Find_ID(Idol))))
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
	if (!FindObject(Find_Or(Find_ID(InventorsLab), Find_ID(Idol))))
		return;	
	var plr = clonk->GetOwner();
	if (GetPlayerType(plr) == C4PT_Script)
		return;
	var new_clonk = CreateObjectAbove(Clonk, 0, 0, plr);
	new_clonk->MakeCrewMember(plr);
	SetCursor(plr, new_clonk);
	var relaunch = CreateObjectAbove(RelaunchContainer, LandscapeWidth() / 2, LandscapeHeight() / 2, new_clonk->GetOwner());
	relaunch->SetRelaunchTime(3);
	relaunch->StartRelaunch(new_clonk);
	SpawnPlayer(plr);
	return;
}

public func SpawnPlayer(int plr)
{
	var clonk = GetCrew(plr);
	clonk->SetPosition(507, 408);	
	clonk->CreateContents(Sword);
	clonk->SetMagicEnergy(50);
	ScheduleCall(clonk, "SelectChampion", 1, 0);
	return;
}

public func OnClonkLeftRelaunch(object clonk)
{
	clonk->Exit();
	clonk->SetPosition(507, 408);
	return;
}


/*-- Waves Control --*/

public func GetAttackWave(int nr)
{
	// The round starts with a short phase to prepare.
	if (nr % 2 == 1) 
		return new DefenseEnemy.BreakWave { Duration = 10 };
	
	// Attack positions.
	var pos_left= {X = 10, Y = 408, Exact = true};
	var pos_right = {X = LandscapeWidth() - 10, Y = 408, Exact = true};
	var pos_above = {X = LandscapeWidth() / 2, Y = 10}; 
	
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
	
	// Add enemy ground troups: swordsman, archer, spearman, grenadier, bomber.
	PushBack(wave.Enemies, new DefenseEnemy.Swordsman {
		Amount = BoundBy(nr / 2 + 1, 1, 20),
		Energy = BoundBy(20 + 5 * nr, 30, 100),
		Position = RandomElement([pos_left, pos_right])
	});
	PushBack(wave.Enemies, new DefenseEnemy.Grenadier {
		Amount = BoundBy(nr / 2 + 1, 1, 20),
		Energy = BoundBy(10 + nr, 20, 50),
		Position = RandomElement([pos_left, pos_right])
	});		
	// Add enemy: boom attack.
	PushBack(wave.Enemies, new DefenseEnemy.BoomAttack {
		Amount = BoundBy(nr / 2 + 1, 1, 20),
		Speed = BoundBy(80 + nr * 5, 100, 250),
		Position = pos_above
	});	
	return wave;
}

// The attackers should go for all things alive.
public func GiveRandomAttackTarget(object attacker)
{
	var target = FindObject(Find_OCF(OCF_Alive), Find_Hostile(attacker->GetController()), Sort_Distance());
	if (target)
		return target;
	return;
}