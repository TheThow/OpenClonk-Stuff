/**
	FireMan
	

	@author 
*/

#include Man

local Special1Spell = BallAttackOrder;
local Special2Spell = BallHomeCall;
local Special3Spell = BallDischarge;

local Special1Cooldown = 12;
local Special2Cooldown = 40;
local Special3Cooldown = 60;

local MaxRange = 300;

func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		var balldist = Distance(clonk->GetX() + x, clonk->GetY() + y, clonk.Ball->GetX(), clonk.Ball->GetY());
		var clonkdist = Distance(0, 0, x, y);
		
		if(balldist < 35 || clonkdist > MaxRange)
		{
			Sound("UI::Error", 0, 50, clonk->GetOwner());
			return 0;
		}
	
		var param = {
			ball = clonk.Ball
		};
		
		if(clonk->LaunchSpell(Special1Spell, x, y, 0, 0, param))
			return 1;
	}
	return 0;
}

func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		var param = {
			ball = clonk.Ball
		};
		
		if(clonk->LaunchSpell(Special2Spell, x, y, 0, 0, param))
			return 1;
	}
	return 0;
}

func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		var param = {
			ball = clonk.Ball
		};
		
		if(clonk->LaunchSpell(Special3Spell, x, y, 0, 0, param))
			return 1;
	}
	return 0;
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

	clonk->Sound("Ball::ball_jump", false, 30);

	for(var i = from; i < to; i+=5)
	{
		var r = 10;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var trailparticles =
		{
			Prototype = Particles_ElectroSpark2(),
			Size = PV_Linear(10,0),
			Rotation = PV_Random(0, 360),
			R = 50,
			G = 50,
			B = 255,
			OnCollision = PC_Bounce(),
		};
	
		CreateParticle("StarSpark", x, y, Cos(i, r), Sin(i, r), 10, trailparticles);
	}
}

func BlockEffect(object clonk, range)
{
	for(var i = 0; i < 360; i+=10)
	{
		var r = range;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var trailparticles =
		{
			Prototype = Particles_ElectroSpark2(),
			Size = PV_Linear(15,0),
			Rotation = PV_Random(0, 360),
			R = 50,
			G = 50,
			B = 255,
		};
	
		CreateParticle("StarSpark", x, y, 0, 0, 10, trailparticles);
	}
	
}

func CanCast(object clonk)
{
	if(clonk.Ball)
	{
		if(GetEffect("Blocked", clonk.Ball) || GetEffect("Discharge", clonk.Ball))
			return false;
	}
	
	return _inherited(clonk);
}

func CanCastSpecial2(object clonk)
{
	if(clonk.Ball)
	{
		if(GetEffect("HomeCall", clonk.Ball))
			return false;
	}

	return true;
}

func InitChamp(clonk)
{
	var ball = clonk->CreateObject(BallsManBall, 0, -15, clonk->GetOwner());
	clonk.Ball = ball;
	ball->SetMaster(clonk);
	
	var props =
	{
		R = 255,
		G = 255,
		B = 255,
		Alpha = 20,
		Size = 2,
		BlitMode = GFX_BLIT_Additive,
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	
	var dummy = CreateObject(Dummy, 0, 0, clonk->GetOwner());
	dummy->SetAction("HangOnto", clonk);
	dummy.Visibility = VIS_Owner;
	
	for(var i = 0; i < 360; i++)
	{
		var x = Sin(i, MaxRange, 1);
		var y = -Cos(i, MaxRange, 1);
		dummy->CreateParticle("Flash", x, y, 0, 0, 0, props);
	}
	
	var eff = AddEffect("Range", clonk, 1, 1, nil, BallsMan);
	eff.d = dummy;
}

func FxRangeTimer(target, fx)
{
	if(fx.d->Contained())
	{
		fx.d->Exit();
		fx.d->SetAction("HangOnto", target);
	}
	
	if(fx.d->GetAction() != "HangOnto")
	{
		fx.d->SetAction("HangOnto", target);
	}
	
	if(!target || target.ChampType != BallsMan)
		return -1;
}

func FxRangeStop(object target, proplist fx, int reason, bool temp)
{
	if(temp)
		return;
	
	fx.d->RemoveObject();
}

func CleanUp(object clonk)
{
	if(GetEffect("Range", clonk))
		RemoveEffect("Range", clonk);
		
	if(clonk.Ball)
		clonk.Ball->KillBall();
}

func FxBallHitStart(target, effect)
{
	effect.props =
	{
		Size = PV_Linear(PV_Random(2,5),0),
		BlitMode = GFX_BLIT_Additive,
		R = 50,
		G = 50,
		B = 255,
		Attach = ATTACH_Front | ATTACH_MoveRelative,
		Alpha = PV_Linear(255, 0)
	};
}

func FxBallHitTimer(object target, proplist effect, int time)
{
	target->CreateParticle("Flash", RandomX(-5, 5), RandomX(-10, 10), 0, 0, 10, effect.props, 2);
	
	if(time > 40)
		return -1;
}

global func AddBallHitEffect()
{
	this->AddEffect("BallHit", this, 20, 1, nil, BallsMan);
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