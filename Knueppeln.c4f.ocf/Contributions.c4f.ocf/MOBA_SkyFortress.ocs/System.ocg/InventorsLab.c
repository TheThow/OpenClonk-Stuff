// Destroy targets when they die.

#appendto InventorsLab

local TopTower;
local BotTower;

local FxNoDMGOnParentExist = new Effect
{
	Damage = func(dmg, cause, plr)
	{
		if (cause == 32)
			return dmg;
		if (Target->GetTopTower() && Target->GetBotTower())
			return 0;
		
		return dmg;
	}
};

func Initialize()
{
	CreateEffect(FxNoDMGOnParentExist, 1, 1);
	return _inherited(...);
}

public func Death()
{
	Fireworks();
	Explode(20);
	return;
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