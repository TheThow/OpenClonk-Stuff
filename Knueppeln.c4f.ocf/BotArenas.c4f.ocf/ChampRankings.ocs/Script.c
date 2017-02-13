/**
	Champ Rankings
	Scenario that tests the champs in 1 vs. 1 battles and ranks them.
	
	@author Maikel
*/


// Script players to control the champs.
static script_enemy1;
static script_enemy2;

// Which champs to test and how many battles between each pair.
static const rank_champs = [GreatLeaderMan, ArchMan];
//static const rank_champs = [ArchMan, BallsMan, BatMan, BombMan, EarthMan, ElectroMan, FireMan, FlashMan, GreatLeaderMan, HookMan, IceMan, LaserMan, NinjaMan, TimeMan];
static const nr_pair_battles = 50;


protected func Initialize()
{
	// Script player functions as an opponent.
	CreateScriptPlayer("Enemy 1", 0xffff0000, nil, CSPF_FixedAttributes | CSPF_NoEliminationCheck | CSPF_Invisible);
	CreateScriptPlayer("Enemy 2", 0xff000000, nil, CSPF_FixedAttributes | CSPF_NoEliminationCheck | CSPF_Invisible);
	
	// No friendly fire rule will give different results.
	//CreateObject(Rule_NoFriendlyFire);
	return;
}


/*-- Player Initialization --*/

protected func InitializePlayer(int plr)
{
	// Make all players hostile to each other.
	for (var plr1 in GetPlayers())
		for (var plr2 in GetPlayers())
			if (plr1 != plr2)
			{
				SetHostility(plr1, plr2, true, true);			
				SetHostility(plr2, plr2, true, true);
			}
			
	// Initialize script players differently.	
	if (GetPlayerType(plr) == C4PT_Script)
		return InitializeScriptPlayer(plr);
	
	// Everything visible to the observer.
	SetFoW(false, plr);
	SetPlayerZoomByViewRange(plr, LandscapeWidth(), LandscapeHeight(), PLRZOOM_Direct | PLRZOOM_Set | PLRZOOM_LimitMax);
	var container = CreateObject(RelaunchContainer, LandscapeWidth() / 2, LandscapeHeight() / 2);
	GetCrew(plr)->Enter(container);
	
	// Add test control effect.
	Scenario->CreateEffect(FxChampRankings, 1, 2);
	return;
}

protected func InitializeScriptPlayer(int plr)
{
	// Remove old crew.
	var index = 0;
	while (GetCrew(plr, index))
	{
		GetCrew(plr, index)->RemoveObject();
		index++;
	}
	
	// Store the script player number.
	if (script_enemy1 == nil)
		script_enemy1 = plr;
	else if (script_enemy2 == nil)
		script_enemy2 = plr;
	return;
}


/*-- Test Control --*/

static const FxChampRankings = new Effect
{
	Name = "FxChampRankings",
	Construction = func()
	{
		this.Interval = 2;
		this.launched = false;
		// Init games to play.
		this.nr_battles = nr_pair_battles;
		this.champ_scores = [];
		this.rank_champs = rank_champs;
		this.game_nr = 0;
		this.current_game = nil;
		this.games = [];
		for (var i = 0; i < GetLength(this.rank_champs); i++)
			for (var j = i + 1; j < GetLength(this.rank_champs); j++)
				PushBack(this.games, [this.rank_champs[i], this.rank_champs[j]]);
		return FX_OK;	
	},
	
	Timer = func(int time)
	{
		if (!this.launched)
		{
			if (this.game_nr >= GetLength(this.games))
			{
				this.nr_battles--;
				if (this.nr_battles <= 0)
					return FX_Execute_Kill;			
				this.game_nr = 0;			
			}
			this.current_game = this.games[this.game_nr];
			this->StartTest(this.current_game[0], this.current_game[1]);
			this.game_nr++;
			this.launched = true;
			return FX_OK;
		}
		if (this->IsTestCompleted())
		{
			this.launched = false;
			var winner = this->GetTestWinner();
			this->GiveWin(this.current_game[winner]);
			Log("Winner is %i.", this.current_game[winner]);
			ClearCurrentTest();
			return FX_OK;
		}
		return FX_OK;
	},
	
	Destruction = func()
	{
		Log("===========================");
		for (var index = 0; index < GetLength(this.rank_champs); index++)
			Log("%i: \t %d", this.rank_champs[index], this.champ_scores[index]);
		Log("===========================");
		return FX_OK;
	},
	
	StartTest = func(id champ1, id champ2)
	{
		Log("Testing %i vs. %i.", champ1, champ2);
		this->CreateChamp(champ1, 120, 258, script_enemy1);
		this->CreateChamp(champ2, 392, 258, script_enemy2);
		return;
	},
	IsTestCompleted = func()
	{
		if (ObjectCount(Find_OCF(OCF_CrewMember), Find_Owner(script_enemy1)) == 0 || ObjectCount(Find_OCF(OCF_CrewMember), Find_Owner(script_enemy2)) == 0)
			return true;
		return false;
	},
	GetTestWinner = func()
	{
		if (ObjectCount(Find_OCF(OCF_CrewMember), Find_Owner(script_enemy1)) > ObjectCount(Find_OCF(OCF_CrewMember), Find_Owner(script_enemy2)))
			return 0;
		return 1;
	},
	ClearCurrentTest = func()
	{
		RemoveAll(Find_Owner(script_enemy1));
		RemoveAll(Find_Owner(script_enemy2));
		RemoveAll(Find_ID(Rule_NoFriendlyFire));
		return;
	},
	GiveWin = func(id champ)
	{
		var index = GetIndexOf(this.rank_champs, champ);
		this.champ_scores[index]++;
		return;	
	},
	
	CreateChamp = func(id champ_type, int x, int y, int plr)
	{
		var enemy = CreateObjectAbove(Clonk, x, y, plr);
		if (!enemy)
			return nil;
		enemy->SetDir(DIR_Right);
		enemy->MakeCrewMember(plr);
		champ_type->InitChamp(enemy);
		enemy.ChampType = champ_type;
		enemy->CreateContents(Sword);
		enemy->SetSkin(Random(4));
		enemy->DoMagicEnergy(enemy.MaxMagic / 1000);
		KnueppelnAI->AddAI(enemy);
		KnueppelnAI->SetMaxAggroDistance(enemy, LandscapeWidth());
		KnueppelnAI->SetGuardRange(enemy, 0, 0, LandscapeWidth(), LandscapeHeight());
		enemy->AddEnergyBar();
		return enemy;
	}
};