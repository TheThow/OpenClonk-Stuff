/**
	ElectroOrb
	

	@author 
*/

local ManaCost = 22;
local SpellDamage = 20;
local Speed = 65;
local Dur = 30;

local Name = "$Name$";
local Description = "$Description$";

local pR = 100;
local pG = 100;
local pB = 100;

local shooter;

local travelprt;

local ReturnFx = new Effect {

	Timer = func() {
	
		var fx = Target->CreateEffect(Target.SpeedFx, 1, 1);
		fx.angle = Angle(0, 0, Target->GetXDir(), Target->GetYDir());
		return -1;
	}
};

local SpeedFx = new Effect {

	Timer = func() 
	{
		if (Target->GetAction() != "Travel")
			return -1;
	
		Target->AddVelocity(this.angle + 180, 10);
		var s = Distance(0, 0, Target->GetXDir(), Target->GetYDir());
		
		if ( s >= Target.Speed - 1)
		{
			Target->SetVelocity(this.angle + 180, Target.Speed);
			return -1;
		}
	}

};

local TravelFx = new Effect {

	Timer = func()
	{
		this.timer++;
		//Target->CreateParticle("Flash", 0, 0, 0, 0, 10, Target.travelprt, 1);
		Target->DrawParticleLine("Flash", this.x - Target->GetX(), this.y - Target->GetY(), 0, 0, 1, 0, 0, 10, Target.travelprt);
		
		if (this.timer == Target.Dur*2 + 5)
		{
			Target->SetAction("Idle");
		}
		
		this.x = Target->GetX();
		this.y = Target->GetY();
	}

};

func Initialize()
{
	SetAction("Travel");
	travelprt = {
		Size = PV_Linear(4, 0),
		Alpha = PV_Linear(50, 0),
		R = pR,
		G = pG,
		B = pB,
	};
	SetRDir(20);
}

func CanBeSucked() { return true; }
func IsReflectable()
{
	return 1;
}

func Launch(object clonk, int x, int y)
{
	SetLightRange(30, 70);
	SetLightColor(RGB(255, 255, 255));

	var angle = Angle(0,0,x,y, 10);
	shooter = clonk;
	SetVelocity(angle, Speed, 10);
	Sound("BatMan::bat_throw", false, 50);
	Sound("BatMan::bat_fly", false, 20, nil, 1);
	AddEffect("HitCheck", this, 1,1, nil,nil, clonk);
	CreateEffect(ReturnFx, 1, Dur);
	var fx = CreateEffect(TravelFx, 1, 1);
	fx.x = GetX();
	fx.y = GetY();
}

public func HitObject(obj)
{
	if(obj->~CanBeHit(this) == false)
		return;
		
	if (obj == shooter)
	{
		Sound("BatMan::bat_collect", false, 50);
		obj->DoMagicEnergy(ManaCost/4);
		RemoveObject();
		return true;
	}
	
	obj->AddBatHitEffect();
	obj->Sound("Hits::ProjectileHitLiving*", false, 50);
	obj->Fling();
	WeaponDamage(obj, SpellDamage);
	RemoveObject();
}


func Hit(xdir, ydir)
{
	OnRemove();
	Sound("Hits::Materials::Metal::LightMetalHit1", false, 50, nil, nil, nil, 50);
	RemoveObject();
}

func Blocked()
{
	SetRDir(30);
	SetAction("Idle");
}


func OnRemove()
{
	var prt2 =	{
		Size = PV_Linear(2, 0),
	    ForceY = GetGravity(),
		Stretch = PV_Speed(1000, 500),
		Rotation = PV_Direction(),
	    R = 255,
	    G = PV_Linear(128,32),
	    B = PV_Random(0, 128, 2),
	    Alpha = PV_Random(255,0,3),
		BlitMode = GFX_BLIT_Additive,
	};
	
	CreateParticle("Flash", 0, 0, PV_Random(-15, 15), PV_Random(-20, 0), 70, prt2, 10);
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
		Speed=1000
	},
};