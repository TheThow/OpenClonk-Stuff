/**
	Arch Man
	@author K-Pone
*/

#include Man


/*-- Spells --*/

local Special1Cooldown = 10;
local Special2Cooldown = 100;
local Special3Cooldown = 400;

local Special1Spell = SimpleArrow;
local Special2Spell = ConeArrow;
local Special3Spell = BulldozerRocket;

func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		if(clonk->LaunchSpell(Special1Spell, x, y, 0, 0))
			return 1;
	}
	return 0;
}

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

func JumpEffect(object clonk, string dir)
{
	var range = 15;
	var particlefx = 
	{
		R = 100, G = 100, B = 100,
		Phase = PV_Random(0,3),
		Rotation = PV_Linear(PV_Random(0,360), PV_Random(0,360)),
		Size = PV_Linear(PV_Random(9,4), 0),
	};
	
	Sound("hookjump");
	
	if (dir == "Up")
	{
		for (var i = 135; i < 225; i += 3)
		{
			clonk->CreateParticle("WoodChip", Sin(i, range), -Cos(i, range), Sin(i, 3), -Cos(i, 3), 20, particlefx);
		}
	}
	
	if (dir == "Left")
	{
		for (var i = 45; i < 135; i += 3)
		{
			clonk->CreateParticle("WoodChip", Sin(i, range), -Cos(i, range), Sin(i, 3), -Cos(i, 3), 20, particlefx);
		}
	}
	
	if (dir == "Down")
	{
		for (var i = 315; i < 405; i += 3)
		{
			clonk->CreateParticle("WoodChip", Sin(i, range), -Cos(i, range), Sin(i, 3), -Cos(i, 3), 20, particlefx);
		}
	}
	
	if (dir == "Right")
	{
		for (var i = 225; i < 315; i += 3)
		{
			clonk->CreateParticle("WoodChip", Sin(i, range), -Cos(i, range), Sin(i, 3), -Cos(i, 3), 20, particlefx);
		}
	}
}

/*
range	range of the block effect
*/
func BlockEffect(object clonk, int range)
{
	var particlefx = 
	{
		Phase = PV_Random(0,3),
		Rotation = PV_Linear(PV_Random(0,360), PV_Random(0,360)),
		Size = PV_Linear(PV_Random(9,4), 0),
	};
	
	for (var i = 0; i < 360; i += 3)
	{
		clonk->CreateParticle("WoodChip", Sin(i, range), -Cos(i, range), Sin(i, 3), -Cos(i, 3), 20, particlefx);
	}
}


/*-- AI --*/

public func IsSpecial1Shot() { return true; }
public func IsSpecial1ShotSpeed() { return Special1Spell.Speed; }

public func IsSpecial2Shot() { return true; }
public func IsSpecial2ShotSpeed() { return Special2Spell.Speed; }
public func IsSpecial2ShotRange() { return Special2Spell.Range; }

public func IsSpecial3Shot() { return true; }
public func IsSpecial3ShotStraight() { return true; }
public func IsSpecial3ShotSpeed() { return Special3Spell.Speed; }
public func IsSpecial3ShotThroughWalls() { return true; }


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