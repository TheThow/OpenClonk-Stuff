/**
	Ninja Man
	@author 
*/

#include Man


/*- Spells --*/

local Special1Spell = PoisonDart;
local Special2Spell = ShadowWalk;
local Special3Spell = CriticalStrike;

local Special2Cooldown = 160;
local Special3Cooldown = 250;

public func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		if(clonk->LaunchSpell(Special1Spell, x, y, 0, 0))
		{
			MakeVisible(clonk);
			return true;
		}
	}
	return false;
}

public func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		if(clonk->LaunchSpell(Special2Spell, x, y, 0, 0))
			return 1;
	}
	return 0;
}

public func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if (!released && !mouseclick && abletocast && !cooldown)
	{
		if (clonk->LaunchSpell(Special3Spell, x, y, 0, 0))
		{
			MakeVisible(clonk);			
			return 1;
		}
	}
	
	return 0;
}

public func LeftClick(object clonk, int x, int y, bool released, bool abletocast)
{
	MakeVisible(clonk);
}

public func MakeVisible(object clonk)
{
	if (GetEffect("*Invisible*", clonk))
		RemoveEffect("*Invisible*", clonk);
	return;
}

public func JumpEffect(object clonk, dir)
{
	var lifetime = 10;
	var dummy = CreateObject(Dummy, clonk->GetX(), clonk->GetY(), clonk->GetOwner());
	dummy.Visibility = VIS_Owner;
	AddEffect("Remove", dummy, 1, lifetime, nil, NinjaMan);

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
		
		var trailparticles =
		{
			Size = PV_Linear(5,0),
			R = 150,
			G = 150,
			B = 150,
			OnCollision = PC_Bounce(),
			Attach = ATTACH_Back,
		};
		
		if (GetEffect("*Invisible*", clonk))
			dummy->CreateParticle("Flash", x - clonk->GetX(), y - clonk->GetY(), Cos(i, r), Sin(i, r), lifetime, trailparticles);
		else
			CreateParticle("Flash", x, y, Cos(i, r), Sin(i, r), lifetime, trailparticles);
	}
}

public func BlockEffect(object clonk, range)
{

	for(var i = 0; i < 360; i+=3)
	{
		var r = range;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var props =
		{
			Size = PV_Linear(5,0),
			R = 150,
			G = 150,
			B = 150,
		};
	
		CreateParticle("Flash", x, y, 0, 0, 10, props);
	}
	
}

public func FxRemoveStop(target)
{
	target->RemoveObject();
}

public func InitChamp(object clonk)
{
	clonk->PushActionSpeed("Walk", 250);
}

public func CleanUp(object clonk)
{
	//clonk->PushActionSpeed("Walk", 200);
	clonk.ActMap.Walk.Speed = clonk.Prototype.ActMap.Walk.Speed;
}


/*-- AI --*/

public func IsSpecial1Shot() { return true; }
public func IsSpecial1ShotSpeed() { return Special1Spell.Speed; }

public func ExecuteAISpecial2Spell(effect fx)
{
	if (!fx.Target)
		return false;
	if (fx.Target->GetEnergy() < 20)
		if (fx.Target->LaunchSpell(Special2Spell, 0, 0, 0, 0))
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