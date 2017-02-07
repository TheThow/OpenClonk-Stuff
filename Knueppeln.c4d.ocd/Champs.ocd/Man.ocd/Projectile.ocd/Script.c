
local Speed = 60;

local ManaCost = 22;
local SpellDamage = 15;

local shooter;

public func IsDangerous4AI() { return true; }

func Initialize()
{
	SetAction("Travel");
	SetClrModulation(RGBa(0,0,0,0));
}

func CanBeSucked() { return true; }

func IsReflectable()
{
	return true;
}

func Launch(object clonk, int x, int y)
{
	var angle = Angle(0,0,x,y, 10);
	shooter = clonk;
	SetVelocity(angle, Speed, 10);
	SetController(clonk->GetController());
	AddEffect("HitCheck", this, 1,1, nil,nil, clonk);
	AddEffect("TheEffect", this, 1, 1, this, Projectile);
}

func FxTheEffectStart(object target, proplist effect, int temporary)
{
	if(temporary)
		return;
		
	this->~InitEffect();
}

func FxTheEffectTimer(object target, proplist effect, int time)
{
	this->~TravelEffect(time);
}

public func HitObject(obj)
{
	if(obj->~CanBeHit(this) == false)
		return;
		
	Hit();
}

public func Hit()
{
	Explode(SpellDamage);
}

func ChargeInterrupted()
{
	RemoveObject();
}

local ActMap = {

	Travel = {
		Prototype = Action,
		Name = "Travel",
		Procedure = DFA_FLOAT,
		NextAction = "Travel",
		Length = 1,
		Delay = 1,
		FacetBase = 1,
		Speed=1000
	},
};

