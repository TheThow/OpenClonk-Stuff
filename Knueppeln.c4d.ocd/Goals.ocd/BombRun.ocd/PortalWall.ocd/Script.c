/**
	EarthWallManager
	Insert description here

	@author 
*/

local ManaCost = 35;

local stones;
local HP = 100;
local Size = 80;

local SpellRange = 100;

local bar;
local team;

func Initialize()
{
	bar = CreateObject(Library_Bars, 0, 0, -1);
	bar->SetBarOffset(0, -50, 100);
	bar->SetBarDimensions(200, 50, 100);
}

func CreateWall(int teamid, int size, int hp)
{
	if(hp)
		HP = hp;
	
	if(size)
		Size = size;
	
	team = teamid;
		
	if(stones != nil)
	{
		Destroy();
	}
	
	stones = CreateArray(360/10);
	
	for(var i = 0; i < 360; i+=8)
	{
		var stone = CreateObject(PortalStone, Cos(i, Size), Sin(i, Size), -1);
		var clr = GetTeamColor(teamid);
		var rgba = SplitRGBaValue(clr);
		stone->SetClrModulation(RGBa(rgba[0], rgba[1], rgba[2], 128));
		
		stones[i] = stone;
		if(stone)
			stone->SetMaster(this);
	}
	
	//AddEffect("Remove", this, 20, Dur, this);
}

func GotDamage(int dmg, int plr)
{
	if(GetEffect("DamageCD", this))
		return;

	HP = HP + dmg/1000;
	
	if(HP <= 0)
		ScheduleCall(this, "Destroy", 1);
	
	if(this)
		AddEffect("DamageCD", this, 20, 5);
}

func Destroy()
{
	for(var s in stones)
	{
		if(s)
			s->RemoveObject();
	}
	Sound("Fire::BlastLiquid2");
}

func FxRemoveStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;
	
	Destroy();
}
