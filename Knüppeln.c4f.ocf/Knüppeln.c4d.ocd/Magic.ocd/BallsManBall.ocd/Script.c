/**
	ElectroOrb
	

	@author 
*/

local SpellDamage = 20;
local Speed = 60;

local Size = 12;

local DischargeSize = 35;
local ChargeDur = 10;
local DischargeDamage = 40;

local ShieldDur = 120;
local ShieldAmount = 15;

local BlockDur = 40;

local pR = 50;
local pG = 50;
local pB = 255;

local BorderBound = C4D_Border_Sides | C4D_Border_Top | C4D_Border_Bottom;

local master;

local ox;
local oy;
local rangedummy;

func Initialize()
{
	SetAction("Travel");
	SetRDir(10);
	//AddEffect("MoveTo", this, 1, 1, this);
	
	ox=GetX();
	oy=GetY();
	
	rangedummy = CreateObject(Dummy, 0, 0, GetOwner());
	rangedummy.Visibility = VIS_Owner;
	rangedummy->SetAction("HangOnto", this);
		var props =
		{
			R = 255,
			G = 0,
			B = 0,
			Alpha = 40,
			Size = 70,
			BlitMode = GFX_BLIT_Additive,
			Rotation = PV_Step(10, 0, 1),
			Attach = ATTACH_Back | ATTACH_MoveRelative
			
		};
	rangedummy->CreateParticle("Shockwave2", 0, 0, 0, 0, 0, props, 1);
}

func SetMaster(object clonk)
{
	master = clonk;
	FollowMaster();
}

func FollowMaster()
{
	AddEffect("FollowMaster", this, 1, 1, this);
}

func ClearEffects()
{
	RemoveEffect("FollowMaster", this);
	RemoveEffect("MoveTo", this);
	RemoveEffect("HomeCall", this);
	RemoveEffect("Idle", this);
}

func AttackOrder(x, y)
{
	ClearEffects();

	Sound("Ball::ball_order", false, 20);
		
	var eff = AddEffect("MoveTo", this, 1, 1, this);
	eff.x = master->GetX() + x;
	eff.y = master->GetY() + y;

	var angle = Angle(GetX(), GetY(), eff.x, eff.y, 10);
	SetVelocity(angle, Speed, 10);
}

func Discharge()
{
	ClearEffects();
	SetXDir(0);
	SetYDir(0);
	
	ox=GetX();
	oy=GetY();
	
	AddEffect("Discharge", this, 1, 1 ,this);
	
	Sound("Ball::ball_start_discharge", false, 20);
}

func HomeCall()
{
	ClearEffects();
	
	Sound("Ball::ball_call", false, 20);
	
	var eff = AddEffect("HomeCall", this, 1, 1, this);
	eff.x = master->GetX();
	eff.y = master->GetY();
	
	var angle = Angle(GetX(), GetY(), eff.x, eff.y, 10);
	SetVelocity(angle, Speed, 10);
}

func FxDischargeStart(object target, proplist effect, int temporary)
{
	for(var i = 0; i < 360; i+= 10)
	{
		var props = {
			Size = PV_Linear(4, 0),
			Stretch = 10000,
			Rotation = i + 180,
		    R = pR,
		    G = pG,
		    B = pB,
			BlitMode = GFX_BLIT_Additive,
		};
		
		var x = Sin(i, DischargeSize + RandomX(-2, 2));
		var y = -Cos(i, DischargeSize + RandomX(-2, 2));
		
		var rnd = RandomX(4,20);
		var xdir = Sin(i + 180, rnd);
		var ydir = -Cos(i + 180, rnd);
		
		CreateParticle("Flash", x, y, xdir, ydir, 25, props, 1);
	}
	
}

func FxDischargeTimer(object target, proplist effect, int time)
{
	if(time >= ChargeDur)
		return -1;
}

func FxDischargeStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;
		
	Sound("Ball::ball_discharge", false, 70);
		
	var flashparticle =
	{
		Alpha = 100,
		Size = DischargeSize * 2,
		R = pR,
		G = pG,
		B = pB,
		Rotation = PV_Random(0,360),
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("StarSpark", 0, 0, 0, 0, 10, flashparticle, 5);
	
	for(var o in FindObjects(Find_Distance(DischargeSize), Find_Func("CanBeHit")))
	{
		var angle = Angle(GetX(), GetY(), o->GetX(), o->GetY());
		
		o->Fling(Sin(angle, 8), -Cos(angle, 8));
		WeaponDamage(o, DischargeDamage);
	}
	
	for(var r = 5; r < DischargeSize; r += 5)
	{
		for(var i = 0; i < 360; i+= 1)
		{
			var props = {
				Size = PV_Linear(4, 0),
				Rotation = PV_Random(0, 360),
			    R = pR,
			    G = pG,
			    B = pB,
			    Alpha = PV_Linear(255,0),
				BlitMode = GFX_BLIT_Additive,
			};
			
			var x = Sin(i, r + RandomX(-2, 2));
			var y = -Cos(i, r + RandomX(-2, 2));
			
			
			CreateParticle("StarSpark", x, y, 0, 0, 25, props, 2);
		}
	}
	
	var props = {
		Size = PV_Linear(20, 200),
	    R = pR,
	    G = pG,
	    B = pB,
	    Alpha = PV_Linear(180, 0),
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("Shockwave2", 0, 0, 0, 0, 10, props, 1);
	Sound("Ball::ball_after_discharge", false, 30);
	
	Idle();
}

func FxFollowMasterTimer(object target, proplist effect, int time)
{
	if(!master)
	{
		KillBall();
		return -1;
	}

	MoveToPos(master->GetX(), master->GetY() - 15);
	
	var trailparticles2 =
	{
		Size = PV_Linear(5,0),
		BlitMode = GFX_BLIT_Additive,
		R = 50,
		G = 50,
		B = 50,
		Attach=ATTACH_Back,
	};
	
	DrawParticleLine("Flash", 0, 0, ox-GetX(), oy-GetY(), 1, 0, 0, 15, trailparticles2);
	ox=GetX();
	oy=GetY();
}

func FxHomeCallTimer(object target, proplist fx, int time)
{
	if(!master)
	{
		KillBall();
		return -1;
	}	
	
	if(GetEffect("Blocked", this))
	{
		ox=GetX();
		oy=GetY();
		return;
	}
		
	var trailparticles =
	{
		Size = PV_Linear(5,0),
		BlitMode = GFX_BLIT_Additive,
		R = 0,
		G = 255,
		B = 255,
		Attach=ATTACH_Back,
	};
	
	DrawParticleLine("Flash", 0, 0, ox-GetX(), oy-GetY(), 1, 0, 0, 15, trailparticles);
	
	var trailparticles2 =
	{
		Size = PV_Linear(2,0),
		BlitMode = GFX_BLIT_Additive,
		R = 0,
		G = 255,
		B = 255,
		Attach=ATTACH_Back,
	};
	
	if(time%7 == 0)
	{
		for(var i = 0; i < 360; i+=5)
		{
			CreateParticle("Flash", Sin(i, 3), -Cos(i, 5), 0, 0, 10, trailparticles2, 2);
		}
	}

	fx.x = master->GetX();
	fx.y = master->GetY();
	var angle = Angle(GetX(), GetY(), fx.x, fx.y, 10);
	var txdir = Sin(angle, Speed + 10, 10);
	var tydir = -Cos(angle, Speed + 10, 10);
	SetXDir((GetXDir() + (txdir - GetXDir())/2));
	SetYDir((GetYDir() + (tydir - GetYDir())/2));
	
	CheckForEnemies();
	
	ox=GetX();
	oy=GetY();
	
	var dst = Distance(GetX(), GetY(), fx.x, fx.y);
	if(dst < 8)
	{
		AddShield(master);
		Sound("Ball::ball_shield", false, 20);
		
		var particles =
		{
			Prototype = Particles_Glimmer(),
			R = pR,
			G = pG,
			B = pB,
			Alpha = 255,
			Size = PV_Linear(10, 0),
			OnCollision = PC_Bounce(),
		};
		CreateParticle("StarSpark", 0, 0, PV_Random(-60,60), PV_Random(-60, 60), 25, particles, 5);
		
		var particle =
		{
			Alpha = PV_Linear(255, 0),
			Size = 50,
			R = pR,
			G = pG,
			B = pB,
			BlitMode = GFX_BLIT_Additive,
		};
		master->CreateParticle("StarSpark", 0, 0, 0, 0, 7, particle, 4);
		
		FollowMaster();
		return -1;
	}
}



func FxMoveToTimer(object target, proplist fx, int time)
{
	if(!master)
	{
		KillBall();
		return;
	}
		
	if(GetEffect("Blocked", this))
	{
		ox=GetX();
		oy=GetY();
		return;
	}
		
	var trailparticles =
	{
		Size = PV_Linear(5,0),
		BlitMode = GFX_BLIT_Additive,
		R = pR,
		G = pG,
		B = pB,
		Attach=ATTACH_Back,
	};
	
	DrawParticleLine("Flash", 0, 0, ox-GetX(), oy-GetY(), 1, 0, 0, 15, trailparticles);
	
	
	var trailparticles2 =
	{
		Size = PV_Linear(2,0),
		BlitMode = GFX_BLIT_Additive,
		R = pR,
		G = pG,
		B = pB,
		Attach=ATTACH_Back,
	};
	
	if(time%7 == 0)
	{
		for(var i = 0; i < 360; i+=5)
		{
			CreateParticle("Flash", Sin(i, 3), -Cos(i, 5), 0, 0, 10, trailparticles2, 2);
		}
	}

	MoveToPos(fx.x, fx.y);
	
	ox=GetX();
	oy=GetY();
	
	var dst = Distance(GetX(), GetY(), fx.x, fx.y);
	if(dst < 5)
	{
		SetXDir(0);
		SetYDir(0);
		CheckForEnemies();
		Idle();
		
		
		var flashparticle =
		{
			Alpha = 100,
			Size = Size * 2,
			R = pR,
			G = pG,
			B = pB,
			Rotation = PV_Random(0,360),
			BlitMode = GFX_BLIT_Additive,
		};
		CreateParticle("StarSpark", 0, 0, 0, 0, 10, flashparticle, 5);
		
		return -1;
	}
}

func Idle()
{
	ClearEffects();
	ox=GetX();
	oy=GetY();
	AddEffect("Idle", this, 1, 1, this);
}

func FxIdleTimer()
{
	if(!master)
	{
		KillBall();
		return;
	}
}

func MoveToPos(x, y, add)
{
	var angle = Angle(GetX(), GetY(), x, y, 10);
	var txdir = Sin(angle, Speed + add, 10);
	var tydir = -Cos(angle, Speed + add, 10);
	SetXDir((GetXDir() + (txdir - GetXDir())/10));
	SetYDir((GetYDir() + (tydir - GetYDir())/10));
}


func CheckForEnemies()
{
	for(var o in FindObjects(Find_Distance(Size), Find_Func("CanBeHit")))
	{
		if(o->GetOwner() == GetOwner() || GetEffect("BallHitCD", o))
			continue;
			
		o->Fling(0, -2);
		AddEffect("BallHitCD", o, 1, 15);
		
		var trailparticles =
		{
			Prototype = Particles_ElectroSpark2(),
			Size = PV_Linear(PV_Random(5,15),0),
			BlitMode = GFX_BLIT_Additive,
			Rotation = PV_Random(0,360),
			R = pR,
			G = pG,
			B = pB,
		};
		
		CreateParticle("Lightning", o->GetX() - GetX(), o->GetY() - GetY(), 0, 0, 10, trailparticles, 5);
		
		WeaponDamage(o, SpellDamage);
		Sound("Ball::ball_hit", false, 50);
	}
}

func IsReflectable(object clonk)
{
	if(clonk == master || GetEffect("Blocked", this) || GetEffect("FollowMaster", this) || GetEffect("Discharge", this))
		return 0;
		
	return 1;
}

func Blocked(object clonk)
{
	AddEffect("Blocked", this, 1, 1, this);
	Sound("Ball::ball_blocked", false, 20);
	
	var objectangle = Angle(clonk->GetX(), clonk->GetY(), GetX(), GetY());
	
	//var tangle = 2* ( (objectangle + 90)%360 - entryangle) + entryangle;
	SetVelocity(objectangle, Speed);
	Idle();
}

func FxBlockedTimer(object target, proplist fx, int time)
{
	if(time >= BlockDur)
		return -1;
	
	var lightning =
	{
		Prototype = Particles_ElectroSpark2(),
		Size = PV_Linear(PV_Random(3,6),0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Random(0,360),
		R = 100,
		G = 100,
		B = pB,
		Attach = ATTACH_Front,
	};
	
	target->CreateParticle("Lightning", RandomX(-6, 6), RandomX(-6, 6), 0, 0, 20, lightning, 3);
}

func FxBlockedStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;
		
	SetXDir(0);
	SetYDir(0);
	
	ox=GetX();
	oy=GetY();
	
	Sound("Ball::ball_resume", false, 20);
}



func AddShield(object clonk)
{
	AddEffect("BallShield", clonk, 1, 1, this); 
}

func FxBallShieldStart(object target, proplist effect, int temporary)
{
	effect.dummy = CreateObject(Dummy, target->GetX(), target->GetY(), target->GetOwner());
	effect.dummy.Visibility = VIS_All;
	effect.dummy->SetAction("HangOnto", target);
	effect.shield = ShieldAmount;
	effect.cancel = false;
	
	var props =
		{
			R = 0,
			G = 0,
			B = 255,
			Alpha = 255,
			Size = 40,
			BlitMode = GFX_BLIT_Additive,
			Rotation = PV_Step(10, 0, 1),
			Attach = ATTACH_Back | ATTACH_MoveRelative
			
	};
	effect.dummy->CreateParticle("Shockwave", 0, 0, 0, 0, 0, props);
}

func FxBallShieldTimer(object target, proplist fx, int time)
{
	if(time > ShieldDur || fx.cancel)
		return -1;
}

func FxBallShieldDamage(object target, proplist effect, int damage, int cause)
{
	if(damage < 0)
	{
		effect.shield += damage/1000;
	
		if(effect.shield > 0)
			return 0;
		
		effect.cancel = true;
		return effect.shield*1000;
	}
	
	return damage;
}

func FxBallShieldStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;
		
	if(effect.dummy)
		effect.dummy->RemoveObject();
		
	if(!target)
		return;
}

func KillBall()
{
	Sound("Ball::ball_die", false, 50);
	if(rangedummy)
		rangedummy->RemoveObject();
	RemoveObject();
}

local ActMap = {

	Travel = {
		Prototype = Action,
		Name = "Travel",
		Procedure = DFA_FLOAT,
		NextAction = "Travel",
		Length = 1,
		Delay = 1,
		FacetBase = 1,
		StartCall = "Traveling",
		Speed=1000
	},
};