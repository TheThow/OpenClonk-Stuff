/**
	EarthWallManager
	Insert description here

	@author 
*/

local ManaCost = 35;

local stones;
local HP = 100;
local Dur = 10000;

local SpellRange = 100;

func Initialize()
{
	
}

func CreateWall(int hp)
{
	if(hp)
		HP = hp;
	
	stones = CreateArray(360/10);
	
	for(var i = 0; i < 360; i+=8)
	{
		var stone = CreateObject(EarthStone, Cos(i, 80), Sin(i, 80), -1);
		stones[i] = stone;
		if(stone)
			stone->SetMaster(this);
	}
	
	AddEffect("Remove", this, 20, Dur, this);
}

func GotDamage(int dmg)
{
	if(GetEffect("DamageCD", this))
		return;

	HP = HP + dmg/1000;
	
	if(HP <= 0)
		Destroy();
	
	if(this)
		AddEffect("DamageCD", this, 20, 1);
}

func Destroy()
{
	for(var s in stones)
	{
		if(s)
			s->RemoveObject();
	}
	Sound("Fire::BlastLiquid2");
	RemoveObject();
}

func FxRemoveStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;
	
	Destroy();
}
