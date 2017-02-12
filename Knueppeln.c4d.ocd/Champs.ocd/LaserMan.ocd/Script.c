/**
	Laser Man
	@author 
*/

#include Man


/*-- Spells --*/

local Special1Spell = LaserStar;
local Special2Spell = LaserRay;
local Special3Spell = LaserBlink;

public func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast)
	{
		if(clonk->LaunchSpell(Special1Spell, x, y, 0, 0))
			return true;
	}
	return false;
}

public func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast)
	{
		if(clonk->LaunchSpell(Special2Spell, x, y, 0, 0))
			return true;
	}
	return false;
}

public func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast)
	{
		if(clonk->LaunchSpell(Special3Spell, x, y, 0, 0))
			return true;
	}
	return false;
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

	clonk->Sound("electro_shot", false, 20, nil, nil, nil, 200);

	for(var i = from; i < to; i+=5)
	{
		var r = 10;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var trailparticles =
		{
			Size = PV_Linear(2,0),
			Rotation = PV_Random(0, 360, 1),
			G = 0, B = 0,
			Stretch = PV_Random(10000, 20000),
			BlitMode = GFX_BLIT_Additive
		};
	
		CreateParticle("StarSpark", x, y, Cos(i, r), Sin(i, r), 10, trailparticles);
	}
}

public func BlockEffect(object clonk, range)
{
	var ray = 
	{
		Size = 16,
		Alpha = PV_Linear(255,0),
		BlitMode = GFX_BLIT_Additive,
		G = 0, B = 0
	};
	for (var angle = 0; angle < 360; angle += 10)
	{
		ray.Rotation = angle + 90;
		var from_x = clonk->GetX();
		var from_y = clonk->GetY();
		var off_x = +Sin(angle, range);
		var off_y = -Cos(angle, range);
		var to_x = from_x + off_x;
		var to_y = from_y + off_y;
		CreateParticle("RaySpark", to_x, to_y, 0, 0, 10, ray, 1);
		if (!Random(5))
		{
			ray.Rotation = angle;
			DrawParticleLine("RaySpark", from_x + off_x/4, from_y + off_y/4, from_x + 3 * off_x / 4, from_y + 3 * off_y / 4, 8, 0, 0, 10, ray);	
		}
	}
}

public func FxLaserHitTimer(object target, proplist effect, int time)
{
	var lightning =
	{
		Prototype = Particles_ElectroSpark2(),
		Size = PV_Linear(PV_Random(2,5),0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Random(0,360),
		R = 255,
		G = 0,
		B = 0,
		Attach = ATTACH_Front | ATTACH_MoveRelative,
	};
	
	target->CreateParticle("Lightning", RandomX(-5, 5), RandomX(-10, 10), 0, 0, 10, lightning, 2);
	
	if(time > 40)
		return -1;
}


/*-- AI --*/

public func ExecuteAISpecial1Spell(effect fx)
{
	if (!fx.Target || !fx.target)
		return false;
	var x = fx.Target->GetX(), y = fx.Target->GetY();
	var tx = fx.target->GetX(), ty = fx.target->GetY() - 4;
	if (Distance(x, y, tx, ty) > 30)
		return false;
	if (fx.Target->LaunchSpell(Special1Spell, tx - x, ty - y, 0, 0))
		return true;
	return false;
}

public func ExecuteAISpecial2Spell(effect fx)
{
	if (!fx.Target || !fx.target)
		return false;
	var x = fx.Target->GetX(), y = fx.Target->GetY();
	var tx = fx.target->GetX(), ty = fx.target->GetY() - 4;
	if (Distance(x, y, tx, ty) > Special2Spell.MaxLength)
		return false;
	if (fx.Target->LaunchSpell(Special2Spell, tx - x, ty - y, 0, 0))
		return true;
	return false;
}

public func ExecuteAISpecial3Spell(effect fx)
{
	if (!fx.Target || !fx.target)
		return false;
	var x = fx.Target->GetX(), y = fx.Target->GetY();
	var tx = fx.target->GetX(), ty = fx.target->GetY() - 4;
	if (Distance(x, y, tx, ty) > Special3Spell.Length)
		return false;
	if (fx.Target->LaunchSpell(Special3Spell, tx - x, ty - y, 0, 0))
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
//local PassiveName = "$PassiveName$";
//local PassiveDescription = "$PassiveDescription$";