/**
	Time Man
	@author 
*/

#include Man


/*-- Spells --*/

local Special1Spell = TimeProjectile;
local Special2Spell = TimeTravel;
local Special3Spell = SlowField;

local Special2Cooldown = 5;
local Special3Cooldown = 240;

public func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if (!released && !mouseclick && abletocast && !cooldown)
	{
		if (clonk->LaunchSpell(Special1Spell, x, y, 0, 0))
			return true;
	}
	return false;
}

public func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if (!released && !mouseclick && abletocast && !cooldown)
	{
		if (clonk->LaunchSpell(Special2Spell, x, y, 0, 0))
			return true;
	}
	return false;
}

public func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	var props =
	{
		R = 150,
		G = 120,
		B = 50,
		Alpha = 40,
		Size = Special3Spell.SpellRange*2,
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Step(10, 0, 1),
		Attach = ATTACH_Back | ATTACH_MoveRelative
	};
	return CastSpellWithSpellRange(clonk, x, y, released, mouseclick, abletocast, cooldown, props, Special3Spell);
}

public func JumpEffect(object clonk, dir)
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

	clonk->Sound("time_hit", false, 30);

	for(var i = from; i < to; i+=5)
	{
		var r = 10;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var angle = Angle(0,0,Cos(i, r),Sin(i, r));
		
		var trailparticles =
		{
			Prototype = Particles_ElectroSpark2(),
			Size = PV_Linear(10,0),
			Rotation = angle,
			R = 255,
			G = 165,
			B = 50,
			OnCollision = PC_Bounce(),
		};
	
		CreateParticle("Lightning", x, y, Cos(i, r), Sin(i, r), 10, trailparticles);
	}
}

public func BlockEffect(object clonk, range)
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
			R = 255,
			G = 165,
			B = 50,
		};
	
		CreateParticle("Lightning", x, y, 0, 0, 10, trailparticles);
	}
	
}

public func CleanUp(object clonk)
{
	if(clonk.TimeTravelMark)
		clonk.TimeTravelMark = nil;
}


public func FxTimeHitTimer(object target, proplist effect, int time)
{
	if (!target)
		return FX_Execute_Kill;

	var lightning =
	{
		Prototype = Particles_ElectroSpark2(),
		Size = PV_Linear(PV_Random(2,5),0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Random(0,360),
		R = 255,
		G = 150,
		B = 50,
		Attach = ATTACH_Front | ATTACH_MoveRelative,
	};
	
	target->CreateParticle("Lightning", RandomX(-5, 5), RandomX(-10, 10), 0, 0, 10, lightning, 2);
	
	if (time > 40)
		return FX_Execute_Kill;
	return FX_OK;
}


/*-- AI --*/

public func IsSpecial1Shot() { return true; }
public func IsSpecial1ShotStraight() { return true; }
public func IsSpecial1ShotSpeed() { return Special1Spell.Speed; }

public func ExecuteAISpecial3Spell(effect fx)
{
	if (!fx.Target || !fx.target)
		return false;
	var x = fx.Target->GetX(), y = fx.Target->GetY();
	var tx = fx.target->GetX(), ty = fx.target->GetY();
	var dt = 5;
	tx += fx.control->GetTargetXDir(fx.target, dt);
	ty += fx.control->GetTargetYDir(fx.target, dt);
	if (Distance(x, y, tx, ty) > Special3Spell.SpellRange)
		return false;
	if (fx.Target->LaunchSpecial3(tx - x, ty - y, false, true, fx.Target->CanCast() && this->CanCastSpecial3(fx.Target)))
		return true;
	return false;
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
local PassiveName = "$PassiveName$";
local PassiveDescription = "$PassiveDescription$";