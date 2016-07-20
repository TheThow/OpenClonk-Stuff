/**
	EarthWallManager
	Insert description here

	@author 
*/

local ManaCost = 30;

local stones;
local HP = 50;
local Dur = 220;

local SpellRange = 100;

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
	
	if(hp)
		HP = hp;
	
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
		ScheduleCall(this, "Destroy", 1);
	
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
