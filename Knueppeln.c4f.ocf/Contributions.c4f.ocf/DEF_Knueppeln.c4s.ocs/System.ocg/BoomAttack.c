// Explode on blow.

#appendto DefenseBoomAttack

public func Construction()
{
	this->CreateEffect(FxOnDamage, 100);
	return _inherited(...);
}

local FxOnDamage = new Effect
{
	Damage = func(int damage, int cause, int by_player)
	{
		this.Target->DoFireworks(by_player);
		return damage;
	}
};

public func CanBeHit() { return true; }

public func CatchBlow(int damage, object obj)
{
	return this->DoFireworks(obj->GetController());
}

public func IncinerationEx(int caused_by)
{
	return this->DoFireworks(caused_by);
}

local BlastIncinerate = 1;