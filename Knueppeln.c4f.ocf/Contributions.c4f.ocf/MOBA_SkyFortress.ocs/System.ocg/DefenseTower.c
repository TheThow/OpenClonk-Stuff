
#appendto DefenseTower

local ParentTower;

public func Initialize()
{
	var firing_rate = 50;
	CreateEffect(FxFiringControl, 1, firing_rate, TowerProjectile);
	CreateEffect(FxNoDMGOnParentExist, 1, 1);
	return;
}

public func SetParentTower(object tower)
{
	ParentTower = tower;
}

public func GetParentTower()
{
	return ParentTower;
}

local FxNoDMGOnParentExist = new Effect
{
	Damage = func(dmg, cause, plr)
	{
		if (cause == 32)
			return dmg;
		if (Target->GetParentTower())
			return 0;
		
		return dmg;
	}
};

public func Death()
{
	Fireworks();
	Explode(20);
	return;
}

public func CanBeHit(object by) { return true; }