// Destroy targets when they die.

#appendto InventorsLab

local TopTower;
local BotTower;

local FxNoDMGOnParentExist = new Effect
{
	Construction = func()
	{
		this.LastDMGAnnouncement = 0;
	},
	
	Damage = func(dmg, cause, plr)
	{
		// Damage announcement
		if (Time - this.LastDMGAnnouncement > 350)
		{
			this.LastDMGAnnouncement = Time;
			Target->AnnounceAttack();
		}
		
		if (cause == FX_Call_EngScript)
			return dmg;
		if (Target->GetTopTower() && Target->GetBotTower())
			return 0;
		
		return dmg;
	}
};

func Initialize()
{
	this->CreateEffect(FxNoDMGOnParentExist, 1, 1);
	return _inherited(...);
}

public func Death()
{
	Fireworks();
	Explode(20);
	return;
}

public func AnnounceAttack()
{
	for(var i = 0; i < GetPlayerCount(); i++)
	{
		var plr = GetPlayerByIndex(i);
		if (GetPlayerType(plr) == C4PT_Script) continue; // Minion Players have no crew and need no announcements
		if (!Hostile(this->GetOwner(), plr))
		{
			var crew = GetCrew(plr);
			Log("%v for %d", crew, plr);
			crew->Sound("$SndYourBaseUnderAttack$", true, 100, plr);
		}
	}
}

public func SetTopTower(tower)
{
	TopTower = tower;
}

public func GetTopTower()
{
	return TopTower;
}

public func SetBotTower(tower)
{
	BotTower = tower;
}

public func GetBotTower()
{
	return BotTower;
}

public func CanBeHit(object by) { return true; }