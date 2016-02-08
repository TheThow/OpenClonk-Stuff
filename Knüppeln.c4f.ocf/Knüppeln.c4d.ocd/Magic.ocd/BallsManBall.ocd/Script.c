/**
	ElectroOrb
	

	@author 
*/

local SpellDamage = 15;
local Speed = 50;

local Size = 15;

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
	AddEffect("FollowMaster", this, 1, 1, this);
}

func AttackOrder(x, y)
{
	RemoveEffect("FollowMaster", this);
	Sound("ball_order", false, 50);
	
	if(GetEffect("MoveTo", this))
		RemoveEffect("MoveTo", this);
		
	var eff = AddEffect("MoveTo", this, 1, 1, this);
	eff.x = master->GetX() + x;
	eff.y = master->GetY() + y;
	
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

func MoveToPos(x, y)
{
	var angle = Angle(GetX(), GetY(), x, y, 10);
	var txdir = Sin(angle, Speed, 10);
	var tydir = -Cos(angle, Speed, 10);
	SetXDir((GetXDir() + (txdir - GetXDir())/10));
	SetYDir((GetYDir() + (tydir - GetYDir())/10));
}


func CheckForEnemies()
{
	for(var o in FindObjects(Find_Distance(Size), Find_Func("CanBeHit")))
	{
		if(o->GetOwner() == GetOwner())
			continue;
			
		o->Fling();
		
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

func IsReflectable()
{
	return 1;
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