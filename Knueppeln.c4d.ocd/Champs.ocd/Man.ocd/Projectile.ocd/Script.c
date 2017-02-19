/**
	Projectile
	@author
*/


/*-- Projectile Properties --*/

local Speed = 60;
local ManaCost = 22;
local SpellDamage = 15;


/*-- Code --*/

local shooter;

public func Initialize()
{
	SetAction("Travel");
	SetClrModulation(RGBa(0, 0, 0, 0));
}

public func IsDangerous4AI() { return true; }
public func CanBeSucked() { return true; }
public func IsReflectable() { return true; }
public func IsProjectileSpell() { return true; }

public func Launch(object clonk, int x, int y)
{
	var angle = Angle(0, 0, x, y, 10);
	shooter = clonk;
	SetVelocity(angle, Speed, 10);
	SetController(clonk->GetController());
	AddEffect("HitCheck", this, 1, 1, nil, nil, clonk);
	AddEffect("TheEffect", this, 1, 1, this, Projectile);
}

public func FxTheEffectStart(object target, proplist effect, int temporary)
{
	if (temporary)
		return;
	this->~InitEffect();
}

public func FxTheEffectTimer(object target, proplist effect, int time)
{
	this->~TravelEffect(time);
}

public func HitObject(obj)
{
	if (!obj->~CanBeHit(this))
		return;
	Hit();
}

public func Hit()
{
	Explode(this.SpellDamage);
}

public func ChargeInterrupted()
{
	RemoveObject();
}


/*-- Properties --*/

local ActMap = {
	Travel = {
		Prototype = Action,
		Name = "Travel",
		Procedure = DFA_FLOAT,
		NextAction = "Travel",
		Length = 1,
		Delay = 1,
		FacetBase = 1,
		Speed = 1000
	},
};
