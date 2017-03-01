/**
	Bomb Man
	@author 
*/

#include Man


/*-- Spells --*/

local Special1Spell = StickyBomb;
local Special3Spell = StickyBombMagnet;

public func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if (!released && !mouseclick && abletocast && !cooldown)
	{
		if (clonk->LaunchSpell(Special1Spell, x, y, 0, 0))
			return true;
	}
	return false;
}

public func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast,  bool cooldown)
{
	if (released || mouseclick || cooldown) return false;
	return BlowUpBombs(clonk);
}

public func BlowUpBombs(object clonk)
{
	var existing = FindObjects(Find_ID(StickyBomb), Find_Owner(clonk->GetOwner()));
	if (existing)
	{
		SoundAt("UI::Click");
		for (var bomb in existing)
			bomb->BlowUp();
		return true;
	}
	return false;
}

public func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	var props =
	{
		Alpha = 40,
		Size = Special3Spell.SpellRange*2,
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Step(10, 0, 1),
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	props = Particles_Colored(props, GetPlayerColor(clonk->GetOwner()));
	return CastSpellWithSpellRange(clonk, x, y, released, mouseclick, abletocast, cooldown, props, Special3Spell);
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

	clonk->Sound("Fire::Inflame", false, 30, nil, nil, nil, -100);
	
	var trailparticles =
	{
		Size = PV_Linear(0, 20),
		Alpha = PV_Linear(255, 0),
		OnCollision = PC_Die(),
		CollisionVertex = 0,
		DampingX = 900, DampingY = 900,
		BlitMode = GFX_BLIT_Additive
	};
	trailparticles = Particles_Colored(trailparticles, GetPlayerColor(clonk->GetOwner()));
	for(var i = from; i < to; i+=5)
	{
		var r = 10;
		var x = Cos(i, r);
		var y = Sin(i, r);
		clonk->CreateParticle("Shockwave", x, y, Cos(i, r), Sin(i, r), 60, trailparticles, 2);
	}
}

public func BlockEffect(object clonk, int radius)
{
	var trailparticles =
	{
		Size = PV_KeyFrames(0, 0, 0, 200, PV_Random(10, 25), 1000, 0),
		Alpha = PV_Linear(255, 0),
		OnCollision = PC_Die(),
		CollisionVertex = 0,
		DampingX = 900, DampingY = 900,
		BlitMode = GFX_BLIT_Additive
	};
	trailparticles = Particles_Colored(trailparticles, GetPlayerColor(clonk->GetOwner()));
	
	clonk->CreateParticle("Shockwave",
		PV_Sin(PV_Random(0, 360, 0, 1), PV_Random(radius - 1, radius + 1, 0, 2)),
		PV_Cos(PV_Random(0, 360, 0, 1), PV_Random(radius - 1, radius + 1, 0, 2)),
		PV_Random(-5, 5), PV_Random(-5, 5), PV_Random(20, 30), trailparticles, 100);
}

public func CleanUp(object clonk)
{
	for (var bomb in FindObjects(Find_ID(StickyBomb), Find_Owner(clonk->GetOwner())))
		bomb->Remove();
}


/*-- AI --*/

public func ExecuteAISpecial1Spell(effect fx)
{
	if (!fx.Target || !fx.target)
		return false;
	var x = fx.Target->GetX(), y = fx.Target->GetY();
	var tx = fx.target->GetX(), ty = fx.target->GetY() - 4;
	if (Distance(x, y, tx, ty) > 50)
		return false;
	if (fx.Target->LaunchSpecial1(tx - x, ty - y, false, false, fx.Target->CanCast() && fx.Target.ChampType->CanCastSpecial1(fx.Target)))
		return true;
	return false;
}

public func ExecuteAISpecial2Spell(effect fx)
{
	if (!fx.Target)
		return false;
	// Don't blow up if ally is too close.	
	for (var bomb in FindObjects(Find_ID(StickyBomb), Find_Owner(fx.Target->GetOwner())))
		if (bomb->FindObject(Find_OCF(OCF_Alive), Find_Allied(fx.Target->GetOwner()), Find_Distance(StickyBomb.BlastRadius)))
			return false;
	// Blow up if enemy is close.
	for (var bomb in FindObjects(Find_ID(StickyBomb), Find_Owner(fx.Target->GetOwner())))
	{
		if (bomb->FindObject(Find_OCF(OCF_Alive), Find_Hostile(fx.Target->GetOwner()), Find_Distance(StickyBomb.BlastRadius)))
		{
			BlowUpBombs(fx.Target);
			return true;	
		}
	}
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