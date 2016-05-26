#appendto Boompack

public func IsCarryHeavy() { return false; }
public func CanBeHit(object o) { if(Contained())return false; return true; }

local probability = 35;

func Initialize()
{
	AddEffect("CheckDmg", this, 1, 100, this);
	return _inherited();
}

func FxCheckDmgTimer() { return 0; }

func FxCheckDmgDamage() 
{ 
	if(!GetEffect("Flight", this))
	{ 
		AddEffect("HitCheck", this, 1, 2, nil, nil); 
		Fuse(); 
	}

}

func IsProjectileTarget(object o) { return o->GetID() != Sword; }
func IsKnueppelItem() { return true; }