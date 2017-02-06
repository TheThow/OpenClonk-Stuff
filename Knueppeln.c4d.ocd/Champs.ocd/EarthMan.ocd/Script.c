/**
	FireMan
	

	@author 
*/

#include Man

local Special1Spell = EarthProjectile;
local Special2Spell = EarthWall;
local Special3Spell = DeathRock;

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
public func IsSpecial1ShotSpeed() { return Special1Spell.Speed; }

func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	var props =
	{
		R = 50,
		G = 255,
		B = 50,
		Alpha = 40,
		Size = EarthWall.SpellRange*2,
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Step(10, 0, 1),
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	
	return CastSpellWithSpellRange(clonk, x, y, released, mouseclick, abletocast, cooldown, props, Special2Spell);
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
			Size = PV_Linear(8,0),
			BlitMode = GFX_BLIT_Additive,
			R = 50,
			G = 255,
			B = 50,
		};
		if(i%2 == 0)
			CreateParticle("Flash", x, y, 0, 0, 10, trailparticles, 5);
		
		var smoke =
		{
			Alpha = 255,
			Size = PV_Linear(7,0),
			DampingX = 900, DampingY = 900,
			R = 150, G = 120, B = 100,
			Phase = PV_Random(0, 15)
		};
		CreateParticle("Smoke", x, y, PV_Random(-2,2), PV_Random(-2,2), 10, smoke, 2);
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
			Size = PV_Linear(8,0),
			BlitMode = GFX_BLIT_Additive,
			R = 50,
			G = 255,
			B = 50,
		};
		if(i%10==0 || !Random(4))
			CreateParticle("Flash", x, y, 0, 0, 10, trailparticles, 5);
		
		var smoke =
		{
			Alpha = 255,
			Size = PV_Linear(7,0),
			DampingX = 900, DampingY = 900,
			R = 150, G = 120, B = 100,
			Phase = PV_Random(0, 15)
		};
		CreateParticle("Smoke", x, y, PV_Random(-2,2), PV_Random(-2,2), 10, smoke, 4);
	}
}

func FxEarthHitTimer(object target, proplist effect, int time)
{
	var lightning =
	{
		Prototype = Particles_ElectroSpark2(),
		Size = PV_Linear(PV_Random(2,5),0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Random(0,360),
		R = 50,
		G = 255,
		B = 50,
		Attach = ATTACH_Front | ATTACH_MoveRelative,
	};
	
	target->CreateParticle("Lightning", RandomX(-5, 5), RandomX(-10, 10), 0, 0, 10, lightning, 2);
	
	if(time > 40)
		return -1;
}

global func AddEarthHitEffect()
{
	this->AddEffect("EarthHit", this, 20, 1, nil, EarthMan);
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