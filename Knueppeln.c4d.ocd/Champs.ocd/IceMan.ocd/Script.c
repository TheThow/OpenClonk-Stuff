/**
	Ice Man
	@author
*/

#include Man


/*-- Spells --*/

local Special1Spell = IceProjectile;
local Special2Spell = IceShard;
local Special3Spell = IceShardUltimate;

local Special3Cooldown = 400;

public func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
		clonk->LaunchSpell(IceProjectile, x, y, 0, 0);
}

public func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast,  bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
		clonk->LaunchSpell(Blizzard, x, y, 0, 0);
	
	return false;
}

public func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if (released || mouseclick) return;
	
	var existing = FindObject(Find_ID(IceShardUltimate), Find_Owner(clonk->GetOwner()), Find_Category(C4D_StaticBack), Find_Func("GetProperty", "is_selected"));
	if (existing)
	{
		existing->FireNow();
		var other = FindObjects(Find_ID(IceShardUltimate), Find_Owner(clonk->GetOwner()), Find_Category(C4D_StaticBack), Sort_Func("GetProperty", "number"));
		if (GetLength(other))
		{
			other[0].is_selected = true;
		}
		return true;
	}
	
	if (abletocast && !cooldown)
	{
		if (clonk->LaunchSpell(IceShardUltimate, x, y, 0, 0))
			return true;
	}
	
	return false;
}

public func JumpEffect(object clonk, dir)
{
	var from, to;

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

	clonk->Sound("Fire::Inflame", false, 30, nil, nil, nil, 200);
	
	var trailparticles =
	{
		R = 255,
		G = 255,
		B = 255,
		Size = PV_Linear(PV_Random(4, 6), 0),
		OnCollision = PC_Die(),
		CollisionVertex = 0,
		ForceX = PV_Linear(0, PV_Random(-20, 20, 10)),
		ForceY = PV_Linear(0, PV_Random(-20, 20, 10)),
		DampingX = 900, DampingY = 900,
		BlitMode = GFX_BLIT_Additive
	};
	
	for(var i = from; i < to; i+=5)
	{
		var r = 10;
		var x = Cos(i, r);
		var y = Sin(i, r);
		clonk->CreateParticle("StarFlash", x, y, Cos(i, r), Sin(i, r), 60, trailparticles, 10);
	}
}

public func BlockEffect(object clonk, int radius)
{
	var border = 
	{
		R = 255,
		G = 255,
		B = 255,
		Size = PV_Linear(PV_Random(4, 6), 0),
		OnCollision = PC_Die(),
		CollisionVertex = 0,
		ForceX = PV_Linear(0, PV_Random(-10, 10, 10)),
		ForceY = PV_Linear(0, PV_Random(-10, 10, 10)),
		DampingX = 900, DampingY = 900,
		BlitMode = GFX_BLIT_Additive
	};
	
	for (var angle = 0; angle < 360; angle += 20)
	{
		clonk->CreateParticle("StarFlash", Sin(angle + RandomX(-5, 5), radius), -Cos(angle + RandomX(-5, 5), radius), 0, 0, PV_Random(20, 30), border, 16);
	}
}

public func CleanUp(object clonk)
{
	var shards = FindObjects(Find_ID(IceShard), Find_Owner(clonk->GetOwner()), Find_Category(C4D_StaticBack));
	for (var shard in shards)
		shard->RemoveObject();
}


/*-- AI --*/

public func IsSpecial1Shot() { return true; }
public func IsSpecial1ShotStraight() { return true; }
public func IsSpecial1ShotSpeed() { return Special1Spell.Speed; }

public func IsSpecial2Shot() { return true; }
public func IsSpecial2ShotSpeed() { return Special2Spell.Speed; }


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