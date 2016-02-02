/**
	EarthWallManager
	Insert description here

	@author 
*/

local ManaCost = 35;

local stones;
local HP = 0;
local Dur = 220;

local MaxDst = 100;

func Initialize()
{
}

func Launch(object clonk, int x, int y)
{
	var angle = Angle(0,0, x, y);
	CreateWall(angle, 8);
	Sound("Fire::BlastLiquid1", false, 50);
}

func CreateWall(int angle, int count, int hp)
{
	stones = CreateArray(count);
	
	HP = hp;
	if(!HP)
		HP = 75;
	
	for(var i = 0; i < count; i++)
	{
		var r = i - count/2;
		var radius = 150;
		var stone = CreateObject(EarthStone, -Sin(angle, radius) + Sin(angle + 3*r, radius), +Cos(angle, radius) -Cos(angle + 3*r, radius), GetOwner());
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

	HP = HP - dmg;
	
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
