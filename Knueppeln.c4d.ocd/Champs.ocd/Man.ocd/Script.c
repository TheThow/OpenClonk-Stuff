/**
	Man
	

	@author 
*/

local Special1Cooldown = 10;
local Special2Cooldown = 10;
local Special3Cooldown = 10;

local Special1Spell = Projectile;
local Special2Spell = Projectile;
local Special3Spell = Projectile;

/*
Parameters: 
clonk 			Calling clonk, 
x		 		mouse x, 
y 				mouse y, 
released		key/mouse event released, 
mouseclick 		mouseclick when the special key is held down, 
abletocast 		clonk is able to cast at the moment, 
cooldown 		spell is on cooldown
*/
func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	return 0;
}

func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	return 0;
}

func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	return 0;
}

/*
dir 	"Up", "Left", "Right", "Down"
*/
func JumpEffect(object clonk, string dir)
{

}

/*
range	range of the block effect
*/
func BlockEffect(object clonk, int range)
{

}

func LeftClick(object clonk, int x, int y, bool released, bool abletocast)
{

}

func InitChamp(object clonk)
{

}

func CanCast(object clonk)
{
	return true;
}

func CanCastSpecial1(object clonk)
{
	return true;
}

func CanCastSpecial2(object clonk)
{
	return true;
}

func CanCastSpecial3(object clonk)
{
	return true;
}

func CleanUp(object clonk)
{

}

func CastSpellWithSpellRange(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown, proplist props, id spell)
{
	if(!released && !mouseclick)
	{
		clonk->ShowSpellRange(clonk, spell, props);
	}
	
	if(released && !mouseclick)
	{
		clonk->CancelShowSpellRange();
	}

	if(!released && mouseclick && abletocast && !cooldown)
	{
		if(!CastSpellWithSpellRangeCondition(clonk, x, y, released, mouseclick, abletocast, cooldown, props, spell))
		{
			Sound("UI::Error", false, 50, clonk->GetOwner());
			return 0;
		}
	
		if (Sqrt(x**2 + y**2) > spell.SpellRange)
		{
			var a = Angle(0,0, x, y, 10);
			var newx = Sin(a, spell.SpellRange, 10);
			var newy = -Cos(a, spell.SpellRange, 10);
			
			if(clonk->LaunchSpell(spell, newx, newy, newx, newy))
				return 1;
			
			return 0;
		}
		
		if(clonk->LaunchSpell(spell, x, y, x, y))
			return 1;
	}
	
	return 0;
}

func CastSpellWithSpellRangeCondition(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown, proplist props, id spell)
{
	return true;
}