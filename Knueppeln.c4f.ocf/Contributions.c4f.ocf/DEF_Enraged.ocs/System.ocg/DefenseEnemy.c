// Alternative champ enemies for Knueppeln.

#appendto DefenseEnemy

private func LaunchEnemyAt(proplist prop_enemy, int wave_nr, int enemy_plr, proplist rect)
{
	if (!prop_enemy.Champ)
		return _inherited(prop_enemy, wave_nr, enemy_plr, rect, ...);
	return LaunchChampAt(prop_enemy, wave_nr, enemy_plr, rect);
}

private func LaunchChampAt(proplist prop_enemy, int wave_nr, int enemy_plr, proplist rect)
{
	if (!prop_enemy.Champ)
		return;	
	// Create enemy (per default a Clonk) at the given location.
	var x = rect.x + Random(rect.wdt);
	var y = rect.y + Random(rect.hgt);
	var enemy = CreateObjectAbove(prop_enemy.Type ?? Clonk, x, y, enemy_plr);
	if (!enemy)
		return nil;
	enemy->MakeCrewMember(enemy_plr);
	enemy->SetController(enemy_plr);
	GameCallEx("OnEnemyCreation", enemy, wave_nr);
	
	// Make champ.
	var champ_type = prop_enemy.Champ;
	if (GetType(champ_type) == C4V_Array)
		champ_type = RandomElement(champ_type);
	champ_type->InitChamp(enemy);
	enemy.ChampType = champ_type;
	enemy->CreateContents(Sword);
	enemy->SetSkin(Random(4));
	
	// Update physical properties.
	UpdateEnemyPhysicals(enemy, prop_enemy);	
	
	// Update magic energy.
	enemy.MaxMagic = (prop_enemy.Magic ?? 100) * 1000;
	enemy->DoMagicEnergy(enemy.MaxMagic / 1000);
	
	// Reward for killing enemy: clunker bounty and points.
	enemy.Bounty = prop_enemy.Bounty;
	enemy.Score = prop_enemy.Score;
	
	// Add AI.
	KnueppelnAI->AddAI(enemy);
	KnueppelnAI->SetMaxAggroDistance(enemy, LandscapeWidth());
	KnueppelnAI->SetGuardRange(enemy, 0, 0, LandscapeWidth(), LandscapeHeight());
	return enemy;	
}


/*-- Enemies --*/

// A champ as enemy.
local Champ = new DefaultEnemy
{
	Name = "$EnemyChamp$",
	Champ = [ArchMan, BallsMan, BatMan, BombMan, EarthMan, ElectroMan, FireMan, FlashMan, GreatLeaderMan, HookMan, IceMan, LaserMan, NinjaMan, TimeMan],
	Energy = 50,
	Magic = 100,
	Bounty = 10,
	Score = 2,
	Skin = CSKIN_Default
};