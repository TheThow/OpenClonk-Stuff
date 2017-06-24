/**
	ComboMan
	

	@author 
*/

#include Man

local Name = "$Name$";
local Description = "$Description$";

local Special1Spell = DummySpell;
local Special2Spell = DummySpell;
local Special3Spell = DummySpell;

local Special11Spell = FireProjectile;
local Special12Spell = FireNado;
local Special13Spell = CriticalStrike;

local Special21Spell = TimeTravel;
local Special22Spell = EarthWall;
local Special23Spell = DummySpell;

local Special31Spell = DummySpell;
local Special32Spell = DummySpell;
local Special33Spell = Disrupter;

local Special1Cooldown = 1;
local Special2Cooldown = 1;
local Special3Cooldown = 1;

local adjusted;



func GetCombo(int n)
{
	return eval(Format("ComboMan.Special%dSpell", n));
}

func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast)
	{
		return DoSpellCombo(clonk, x, y, 1);
	}

	return 0;
}

func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast)
	{
		return DoSpellCombo(clonk, x, y, 2);
	}

	return 0;
}

func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast)
	{
		return DoSpellCombo(clonk, x, y, 3);
	}

	return 0;
}

func CtrlPress(object clonk)
{
	clonk.SpellCombo = 0;
}

func InitChamp(object clonk)
{
	clonk.SpellCombo = 0;
}

func DoSpellCombo(clonk, x, y, index)
{
	clonk.SpellCombo = index + clonk.SpellCombo*10;
	var digits = CheckDigits(clonk.SpellCombo);
	
	if (digits == 1)
	{
		AddEffect("ResetCombo", clonk, 1, 10, nil, ComboMan);
	}
	
	if (digits == 2)
	{
		RemoveEffect("ResetCombo", clonk);
		
		var spell = ComboMan->GetCombo(clonk.SpellCombo);
		
		if (spell == nil)
		{
			clonk.SpellCombo = 0;
			return false;
		}
		
		var xo, yo;
		var range = spell.SpellRange;
		
		if(range != nil)
		{
			xo = x;
			yo = y;
		
			if (Sqrt(x**2 + y**2) > spell.SpellRange)
			{
				var a = Angle(0,0, x, y, 10);
				xo = Sin(a, spell.SpellRange, 10);
				yo = -Cos(a, spell.SpellRange, 10);
			}
			
		}
		
		if(clonk->LaunchSpell(spell, x, y, xo, yo))
		{
			clonk.SpellCombo = 0;
			return 1;
		}
		clonk.SpellCombo = 0;
	}
	
	return false;
}

func CheckDigits(int n)
{
	if (n == nil)
		return 0;

	var digits = 0;
	
	while (n > 0)
	{
		n=n/10;
		digits++;
	}
	
	return digits;
	
}


func JumpEffect(object clonk, dir)
{
	var from;
	var to;

	if (dir == "Up")
	{
		from = 50; 
		to = 130;
	}
	if (dir == "Left")
	{
		from = -30; 
		to = 50;
	}
	if (dir == "Right")
	{
		from = 130; 
		to = 210;
	}
	if (dir == "Down")
	{
		from = 230; 
		to = 310;
	}

	clonk->Sound("Hits::SoftHit2", false, 30);

	for(var i = from; i < to; i+=5)
	{
		var r = 10;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		var rgba = SplitRGBaValue(GetPlayerColor(clonk->GetOwner()));
		
		var trailparticles =
		{
			Prototype = Particles_ElectroSpark2(),
			Size = PV_Linear(10,0),
			Rotation = PV_Random(0, 360),
			R = rgba.R,
			G = rgba.G,
			B = rgba.B,
			OnCollision = PC_Bounce(),
		};
	
		CreateParticle("StarSpark", x, y, Cos(i, r), Sin(i, r), 10, trailparticles);
	}
}

func BlockEffect(object clonk, range)
{
	for(var i = 0; i < 360; i+=8)
	{
		var r = range;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		var rgba = SplitRGBaValue(GetPlayerColor(clonk->GetOwner()));
		
		var trailparticles =
		{
			Prototype = Particles_ElectroSpark2(),
			Size = PV_Linear(15,0),
			Rotation = PV_Random(0, 360),
			R = rgba.R,
			G = rgba.G,
			B = rgba.B,
		};
	
		CreateParticle("StarSpark", x, y, 0, 0, 10, trailparticles);
	}
	
}

func FxResetComboTimer(target)
{
	target.SpellCombo = 0;
	return -1;
}