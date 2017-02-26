/**
	MLG Man
	@author Sayned
*/

#include Man


/*-- Spells --*/

local Special1Cooldown = 8;
local Special2Cooldown = 360;
local Special3Cooldown = 300;

local Special1Spell = SprayAndPray;
local Special2Spell = MountainBrew;
local Special3Spell = NoScope;

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
	if (!released && !mouseclick && abletocast && !cooldown)
	{
		if (clonk->LaunchSpell(Special3Spell, x, y, 0, 0))
			return true;
	}
	return false;
}

public func JumpEffect(object clonk, string dir)
{
	var range = 15;
	var particlefx = 
	{
		R = 255, G = 255, B = 255,
		Size = 10
	};
	
	Sound("UI::Click");
	
	if (dir == "Up")
	{
		for (var i = 135; i < 225; i += 20)
		{
			clonk->CreateParticle("Hitmarker", Sin(i, range), -Cos(i, range), 0, 0, 20, particlefx);
		}
	}
	
	if (dir == "Left")
	{
		for (var i = 45; i < 135; i += 20)
		{
			clonk->CreateParticle("Hitmarker", Sin(i, range), -Cos(i, range), 0, 0, 20, particlefx);
		}
	}
	
	if (dir == "Down")
	{
		for (var i = 315; i < 405; i += 20)
		{
			clonk->CreateParticle("Hitmarker", Sin(i, range), -Cos(i, range), 0, 0, 20, particlefx);
		}
	}
	
	if (dir == "Right")
	{
		for (var i = 225; i < 315; i += 20)
		{
			clonk->CreateParticle("Hitmarker", Sin(i, range), -Cos(i, range), 0, 0, 20, particlefx);
		}
	}
}

public func BlockEffect(object clonk, int range)
{
	var particlefx = 
	{
		Size = 10,
	};
	
	for (var i = 0; i < 360; i += 20)
	{
		clonk->CreateParticle("Hitmarker", Sin(i, range), -Cos(i, range), 0, 0, 20, particlefx);
	}
}


/*-- AI --*/

public func IsSpecial1Shot() { return true; }
public func IsSpecial1ShotStraight() { return true; }
public func IsSpecial1ShotSpeed() { return Special1Spell.Speed; }

public func ExecuteAISpecial2Spell(effect fx)
{
  if (!fx.Target)
    return false;
  if (fx.Target->GetMagicEnergy() > fx.Target->GetMaxMagicEnergy() / 2)
    return false;
  if (fx.Target->LaunchSpell(Special2Spell, 0, 0, 0, 0))
    return true;
  return false;
}

public func IsSpecial3Shot() { return true; }
public func IsSpecial3ShotStraight() { return true; }
public func IsSpecial3ShotSpeed() { return Special3Spell.Speed; }


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