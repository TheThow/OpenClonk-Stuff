/**
	FireMan
	

	@author 
*/

#include Man

local Special1Spell = FireProjectile;
local Special2Spell = FireNado;
local Special3Spell = FireDash;

local Special2Cooldown = 35;
local Special3Cooldown = 400;

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
public func IsSpecial1ShotStraight() { return true; }
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

public func IsSpecial2Shot() { return true; }
public func IsSpecial2ShotStraight() { return true; }
public func IsSpecial2ShotSpeed() { return Special2Spell.Speed; }

func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	var props =
	{
		R = 255,
		G = 215,
		B = 0,
		Alpha = 40,
		Size = FireDash.SpellRange*2,
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Step(10, 0, 1),
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	return CastSpellWithSpellRange(clonk, x, y, released, mouseclick, abletocast, cooldown, props, Special3Spell);
}

func JumpEffect(object clonk, dir)
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

	clonk->Sound("Fire::Inflame", false, 30);

	for(var i = from; i < to; i+=5)
	{
		var r = 10;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var trailparticles =
		{
			Prototype = Particles_FireTrail(),
			Size = PV_Linear(10,0),
			Rotation = PV_Linear(360,0)
		};
	
		CreateParticle("Fire", x, y, Cos(i, r), Sin(i, r), 10, trailparticles);
	}
}

func BlockEffect(object clonk, range)
{
	for(var i = 0; i < 360; i+=5)
	{
		var r = range;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var trailparticles =
		{
			Prototype = Particles_FireTrail(),
			Size = PV_Linear(10,0),
			Rotation = PV_Linear(360,0),
		};
	
		CreateParticle("Fire", x, y, 0, 0, 10, trailparticles, 2);
	}
}

func FxFireHitStart(target, fx)
{
	fx.chaoticspark =
	{
		Size = PV_Linear(1, 0),
		ForceX = PV_KeyFrames(10, 0, PV_Random(-6, 6), 333, PV_Random(-6, -6), 666, PV_Random(6, 6), 1000, PV_Random(-6, 6)),
		ForceY = PV_KeyFrames(10, 0, PV_Random(-8, 5), 333, PV_Random(-8, 5), 666, PV_Random(-10, 10), 1000, PV_Random(-10, 15)),
		Stretch = PV_Speed(1000, 500),
		Rotation = PV_Direction(),
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		R = 255,
		G = PV_Linear(255,200),
		B = PV_Random(0, 100),
		DampingX=950,
		DampingY=950,
		Alpha = PV_Random(100,180),
		BlitMode = GFX_BLIT_Additive
	};
	fx.sharpflame =
	{
		Size = 4,
		R = 255,
		G = PV_KeyFrames(0, 0, 255, 666, 0, 627, 0),
		B = 0,
		Rotation = PV_Random(0, 360),
		Phase = PV_Random(0, 5),
		OnCollision = 0,
		BlitMode = GFX_BLIT_Additive,
		Attach = ATTACH_Front | ATTACH_MoveRelative
	};
}

func FxFireHitTimer(object target, proplist effect, int time)
{
	if(time > 40 || !target)
		return -1;
	
	var x = target->GetX();
	var y = target->GetY();
	
	CreateParticle("Magic", PV_Random(x + -5, x+ 5), PV_Random(y + -10, y + 10), PV_Random(25, -25), PV_Random(-25, 12), 50, effect.chaoticspark);
	
	CreateParticle("FireSharp", RandomX(x + -5, x+ 5), RandomX(y + -10, y + 10), 0, PV_Random(-3,-10), 10, effect.sharpflame, 2);
}

global func AddFireHitEffect()
{
	this->AddEffect("FireHit", this, 20, 1, nil, FireMan);
}

func CastSpellWithSpellRangeCondition(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown, proplist props, id spell)
{
		var solidcheck = true;
		var cx = clonk->GetX();
		var cy = clonk->GetY();
		
		for(var i = 0; i < 360; i++)
		{
			if(GBackSolid(cx + x + Sin(i, 5), cy + y + Cos(i, 5)))
				solidcheck = false;
		}
		
		return solidcheck;
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