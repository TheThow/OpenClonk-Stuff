
local Speed = 60;
local Type = "none";

local ManaCost = 22;
local SpellDamage = 15;

local shooter;

func Initialize()
{
	SetAction("Travel");
	SetRDir(10);
	SetClrModulation(RGBa(0,0,0,0));
}

func CanBeSucked() { return true; }

func IsReflectable()
{
	return true;
}

func Launch(object clonk, int x, int y)
{
	Type = clonk->GetChampType();
	var angle = Angle(0,0,x,y, 10);
	shooter = clonk;
	SetVelocity(angle, Speed, 10);
	AddEffect("HitCheck", this, 1,1, nil,nil, clonk);
	AddEffect("TheEffect", this, 1, 1, this, Projectile);
	
	/*
	if (Type == "Electro")
	{
		Sound("electric_shot", false, 100);
		Sound("electro_travel", false, 50, nil, 1);
		SetLightRange(30, 70);
		SetLightColor(RGB(255, 255, 255));
	}
	
	if (Type == "Fire")
	{
		Sound("Fire::Fireball", false, 100);
		Sound("Fire::FuseLoop", false, 20, nil, 1);
		SetLightRange(30, 70);
		SetLightColor(RGB(255, 100, 0));
	}*/
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

