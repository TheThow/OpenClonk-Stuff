/**
	Bat Man
	@author 
*/

#include Man


/*-- Spells --*/

local Special1Spell = Batarang;
local Special2Spell = Disrupter;
local Special3Spell = BatCall;

local Special2Cooldown = 25;
local Special3Cooldown = 400;

func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		if(clonk->LaunchSpell(Special1Spell, x, y, 0, 0))
			return true;
	}
	return false;
}


func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		if(clonk->LaunchSpell(Special2Spell, x, y, 0, 0))
			return true;
	}
	return false;
}

func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	var props =
	{
		R = 100,
		G = 100,
		B = 100,
		Alpha = 40,
		Size = Special3Spell.SpellRange*2,
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Step(10, 0, 1),
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	return CastSpellWithSpellRange(clonk, x, y, released, mouseclick, abletocast, cooldown, props, Special3Spell);
}

func JumpEffect(object clonk, dir)
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

	Sound("BatMan::bat_jump", false, 20);

	for(var i = from; i < to; i+=5)
	{
		var r = 10;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var angle = Angle(0,0,Cos(i, r),Sin(i, r));
		
		var trailparticles =
		{
			Size = PV_Linear(8,0),
			Rotation = angle,
			R = 255,
			G = 255,
			B = 255,
			OnCollision = PC_Bounce(),
		};
	
		CreateParticle("BatPrt", x, y, Cos(i, r), Sin(i, r), 10, trailparticles);
	}
}

func BlockEffect(object clonk, range)
{
	for(var i = 0; i < 360; i+=10)
	{
		var r = range;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var angle = Angle(0,0,Cos(i, r),Sin(i, r)) + 90;
		
		var trailparticles =
		{
			Size = PV_Linear(10,0),
			Rotation = angle + 90,
			R = 255,
			G = 255,
			B = 255,
		};
	
		CreateParticle("BatPrt", x, y, 0, 0, 10, trailparticles);
	}
	
}

func ShowRange()
{

}

func FxBatHitTimer(object target, proplist effect, int time)
{
	if (!target)
		return FX_Execute_Kill;

	var bats =
	{
		Size = PV_Linear(PV_Random(2,5),0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Step(10, PV_Random(0,360), 1),
		R = 100,
		G = 100,
		B = 100,
		Attach = ATTACH_Front | ATTACH_MoveRelative,
	};
	
	target->CreateParticle("BatPrt", RandomX(-5, 5), RandomX(-10, 10), 0, 0, 30, bats, 2);
	
	if (time > 40)
		return FX_Execute_Kill;
	return FX_OK;
}

private func Definition(proplist def)
{
	def.PictureTransformation = Trans_Mul(Trans_Rotate(-65, 0, 1, 0), Trans_Rotate(-35, 0, 0, 1));
	return _inherited(def, ...);
}


/*-- AI --*/

public func IsSpecial1Shot() { return true; }
public func IsSpecial1ShotStraight() { return true; }
public func IsSpecial1ShotSpeed() { return Special1Spell.Speed; }
public func IsSpecial1ShotRange() { return Special1Spell.Range; }

public func IsSpecial2Shot() { return true; }
public func IsSpecial2ShotSpeed() { return Special2Spell.Speed; }

public func ExecuteAISpecial3Spell(effect fx)
{
	if (!fx.Target || !fx.target)
		return false;
	var x = fx.Target->GetX(), y = fx.Target->GetY();
	var tx = fx.target->GetX(), ty = fx.target->GetY();
	var dt = 10;
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
//local PassiveName = "$PassiveName$";
//local PassiveDescription = "$PassiveDescription$";