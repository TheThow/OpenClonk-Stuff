/**
	ElectroOrb
	

	@author 
*/

local SpellDamage = 15;
local Speed = 55;

local Size = 15;

local ShieldDur = 120;
local ShieldAmount = 15;

local pR = 0;
local pG = 0;
local pB = 255;

local master;

local ox;
local oy;

func Initialize()
{
	SetAction("Travel");
	SetRDir(10);
	//AddEffect("MoveTo", this, 1, 1, this);
	
	ox=GetX();
	oy=GetY();
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
}

func AttackOrder(x, y)
{
	ClearEffects();

	Sound("ball_order", false, 20);
		
	var eff = AddEffect("MoveTo", this, 1, 1, this);
	eff.x = master->GetX() + x;
	eff.y = master->GetY() + y;

	var angle = Angle(GetX(), GetY(), eff.x, eff.y, 10);
	SetVelocity(angle, Speed, 10);
}

func HomeCall()
{
	ClearEffects();
	
	Sound("ball_call", false, 20);
	
	var eff = AddEffect("HomeCall", this, 1, 1, this);
	eff.x = master->GetX();
	eff.y = master->GetY();
	
	var angle = Angle(GetX(), GetY(), eff.x, eff.y, 10);
	SetVelocity(angle, Speed, 10);
}

func FxFollowMasterTimer(object target, proplist effect, int time)
{
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
	fx.x = master->GetX();
	fx.y = master->GetY();
	var angle = Angle(GetX(), GetY(), fx.x, fx.y, 10);
	var txdir = Sin(angle, Speed + 30, 10);
	var tydir = -Cos(angle, Speed + 30, 10);
	SetXDir((GetXDir() + (txdir - GetXDir())/2));
	SetYDir((GetYDir() + (tydir - GetYDir())/2));
	
	CheckForEnemies();
	
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
	
	ox=GetX();
	oy=GetY();
	
	var dst = Distance(GetX(), GetY(), fx.x, fx.y);
	if(dst < 8)
	{
		AddShield(master);
		Sound("ball_shield", false, 20);
		
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
	MoveToPos(fx.x, fx.y);
	CheckForEnemies();
	
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
	
	ox=GetX();
	oy=GetY();
	
	var dst = Distance(GetX(), GetY(), fx.x, fx.y);
	if(dst < 5)
	{
		SetXDir(0);
		SetYDir(0);
		return -1;
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
			
		o->Fling();
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
		
	}
}

func IsReflectable(object clonk)
{
	if(clonk == master)
		return 0;
		
	return 1;
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