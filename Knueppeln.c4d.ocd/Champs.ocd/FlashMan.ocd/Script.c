/**

	FlashMan
	
	
	@author K-Pone

*/

#include Man

local Special1Cooldown = 8;
local Special2Cooldown = 8;
local Special3Cooldown = 400;

local Special1Spell = FlashPrisma;
local Special2Spell = FlashFlinger;
local Special3Spell = SuperSpectralLazor;

func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		if(clonk->LaunchSpell(Special1Spell, x, y, 0, 0))
			return 1;
	}
	return 0;
}

public func IsSpecial1Shot() { return true; }
public func IsSpecial1ShotSpeed() { return Special1Spell.Speed; }

func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		if(clonk->LaunchSpell(Special2Spell, x, y, 0, 0))
			return 1;
	}
	return 0;
}

func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		if(clonk->LaunchSpell(Special3Spell, x, y, 0, 0))
			return 1;
	}
	return 0;
}

/*
dir 	"Up", "Left", "Right", "Down"
*/
func JumpEffect(object clonk, string dir)
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

	Sound("Flash::jump", false, 30);

	for(var i = from; i < to; i+=10)
	{
		var r = 10;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var angle = Angle(0,0,Cos(i, r),Sin(i, r));
		
		var trailparticles =
		{
			Prototype = Particles_MuzzleFlash(),
			Size = PV_Linear(10,0),
			Rotation = angle,
			R = RandomX(0,150),
			G = RandomX(0,150),
			B = RandomX(0,150),
			OnCollision = PC_Die(),
		};
	
		CreateParticle("Flash", x, y, Cos(i, r), Sin(i, r), 15, trailparticles);
	}
}

/*
range	range of the block effect
*/
func BlockEffect(object clonk, int range)
{
	for(var i = 0; i < 360; i+=10)
	{
		var r = range;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var angle = Angle(0,0,Cos(i, r),Sin(i, r)) + 90;
		
		var trailparticles =
		{
			Prototype = Particles_ElectroSpark2(),
			Size = PV_Linear(15,0),
			Rotation = angle,
			R = RandomX(0,200),
			G = RandomX(0,200),
			B = RandomX(0,200),
		};
		
		//CreateParticle("Lightning", x, y, 0, 0, 30, trailparticles);
		//CreateParticle("Lightning", x, y, Cos(i+180, 25), Sin(i+180, 25), 30, trailparticles);
		CreateParticle("Lightning", x, y, Cos(i, 3), Sin(i, 3), 30, trailparticles);
	}
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

func FxFlashHitTimer(object target, proplist effect, int time)
{
	var lightning =
	{
		Prototype = Particles_ElectroSpark2(),
		Size = PV_Linear(PV_Random(2,5),0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Random(0,360),
		R = RandomX(10,60),
		G = RandomX(10,60),
		B = RandomX(10,60),
		Alpha = PV_Linear(100,0),
		Attach = ATTACH_Front | ATTACH_MoveRelative,
	};
	
	target->CreateParticle("Flash", RandomX(-5, 5), RandomX(-10, 10), 0, 0, 10, lightning, 3);
	
	if(time > 25)
		return -1;
}

global func AddFlashHitEffect()
{
	this->AddEffect("FlashHit", this, 20, 1, nil, FlashMan);
}

/*-- Properties --*/

local Name = "$Name$";
local Description = "$Description$";
local Spell1Name = "$Spell1Name$";
local Spell1Description = "$Spell1Description$";
local Spell2Name = "$Spell2Name$";
local Spell2Description = "$Spell2Description$";
local Spell3Name = "$Spell3Name$";
local Spell3Description = "$Spell3Description$";
//local PassiveName = "$PassiveName$";
//local PassiveDescription = "$PassiveDescription$";