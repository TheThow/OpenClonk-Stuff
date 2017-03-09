// Destroy targets when they die.

#appendto InventorsLab

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
		if (FindObject(Find_ID(DefenseTower), Find_Team(Target->GetOwner())))
			return 0;
		
		return dmg;
	}
};

public func Initialize()
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
			crew->Sound("$SndYourBaseUnderAttack$", true, 100, plr);
		}
	}
}

public func CanBeHit(object by) { return true; }