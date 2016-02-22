/**
	ElectroOrb
	

	@author 
*/

local ManaCost = 30;
local SpellDamage = 20;
local Speed = 55;
local Dur = 400;
local Charge_dur = 15;

local length = 20;

local Size = 10;

local Name = "$Name$";
local Description = "$Description$";

local LifeTime = 200;

local Target;

local pR = 255;
local pG = 165;
local pB = 50;

local trailparticles;

func Initialize()
{
	SetAction("Travel");
	SetRDir(10);
	SetClrModulation(RGBa(0,0,0,0));
	
	trailparticles =
	{
		Size = PV_Linear(PV_Random(3, 5),0),
		BlitMode = GFX_BLIT_Additive,
		R = pR,
		G = pG,
		B = pB,
	};
}

func Launch(object clonk, int x, int y)
{
	var params = {
		angle = Angle(0,0,x,y),
		cl = clonk
	};
	clonk->Charge(this, "ChargeStop", Charge_dur, params);
	Target = clonk;
	
	AddEffect("Remove", this, 1, Dur, this);
	
	for(var i = 0; i < 360; i+= 15)
	{
		var chargeprops = {
			Size = PV_Linear(4, 0),
			Stretch = 5000,
			Rotation = i + 180,
		    R = pR,
		    G = pG,
		    B = pB,
			BlitMode = GFX_BLIT_Additive,
		};
	
		var x = Sin(i, 30 + RandomX(-2, 2));
		var y = -Cos(i, 30 + RandomX(-2, 2));
		
		var rnd = RandomX(4,10);
		var xdir = Sin(i + 180, rnd);
		var ydir = -Cos(i + 180, rnd);
		
		clonk->CreateParticle("Flash", 0, 0, xdir, ydir, 25, chargeprops, 1);
	}
}

func ChargeStop(proplist params)
{
	var angle = params.new_angle;
	SetVelocity(angle, Speed, 10);
	
	AddEffect("PlasmaSpear", this, 20, 1 ,this, GetID());
	
	Sound("electro_shot", false, 100);
	Sound("electro_travel", false, 20, nil, 1);
	
	SpearEffect();
	AddEffect("Check", this, 1,1, this);
	
	SetLightRange(30, 70);
	SetLightColor(RGB(pR, pG, pB));
}

func SpearEffect()
{
	ClearParticles();
	
	var angle = Angle(0, 0, GetXDir(), GetYDir());
	
	for(var i = -length; i < length; i+=1)
	{
		var flashparticle =
		{
			Alpha = 255,
			Size = length/4 - Abs(i/4) + 1,
			R = pR,
			G = pG,
			B = pB,
			BlitMode = GFX_BLIT_Additive,
			Attach = ATTACH_Front | ATTACH_MoveRelative
		};
		
		CreateParticle("Flash", Cos(angle-90, i), Sin(angle-90, i), 0, 0, 0, flashparticle, 2);
	}
	
}

func FxCheckTimer(object target, proplist effect, int time)
{
	CreateParticle("Flash", PV_Random(-2,2), PV_Random(-2,2), PV_Random(-7,7), PV_Random(-7,7), 15, trailparticles, 7);
	
	var angle = Angle(0, 0, GetXDir(), GetYDir());
	
	for(var o in FindObjects(Find_Or(Find_Distance(Size, Cos(angle, 8), Sin(angle, 8)), Find_Distance(Size, Cos(angle, -8), Sin(angle, -8))), Find_Func("CanBeHit")))
	{
		if(GetEffect("PlasmaSpearHitCD", o))
			continue;
			
		if(o->GetOwner() == GetOwner() && time < 20)
			continue;
	
		o->Fling(0,-1);
		AddEffect("PlasmaSpearHitCD", o, 1, 15);
		
		Sound("electro_shot", false, 50);
		
		var hitparticles =
		{
			Prototype = Particles_ElectroSpark2(),
			Size = PV_Linear(PV_Random(5,15),0),
			BlitMode = GFX_BLIT_Additive,
			Rotation = PV_Random(0,360),
			R = pR,
			G = pG,
			B = pB,
		};
		
		CreateParticle("Lightning", o->GetX() - GetX(), o->GetY() - GetY(), 0, 0, 10, hitparticles, 5);
		
		WeaponDamage(o, SpellDamage);
	}
}

func Hit(xdir, ydir)
{
	Bounce(xdir, ydir);
	
	if(Distance(0,0, GetXDir(), GetYDir()) > 30)
		Sound("electro_shot", false, 50);
}

func IsReflectable()
{
	return 1;
}


func FxRemoveStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;

	OnRemove();
	
	RemoveObject();
}

func ChargeInterrupted()
{
	RemoveObject();
}

func OnRemove()
{
	var particles =
	{
		Prototype = Particles_Glimmer(),
		R = pR,
		G = pG,
		B = pB,
		Alpha = 255,
		Size = PV_Linear(5, 0),
	};
	CreateParticle("StarSpark", 0, 0, PV_Random(-50,50), PV_Random(-50, 50), 20, particles, 15);
	
	var sphereparticle =
	{
		Alpha = PV_Linear(255, 0),
		Size = 10,
		R = pR,
		G = pG,
		B = pB,
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("StarSpark", 0, 0, 0, 0, 10, sphereparticle, 4);
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

func Bounce(int xdir, int ydir)
{
	var angle = Angle(0, 0, xdir, ydir);
	
	var surface = GetSurfaceVector(0, 0);
	var surface_angle = Angle(0, 0, surface[0], surface[1]);
	var angle_diff = GetTurnDirection(angle - 180, surface_angle);
	var new_angle = surface_angle + angle_diff;
	
	var speed = Distance(0, 0, xdir, ydir);
	SetXDir(Sin(new_angle, speed), 100);
	SetYDir(-Cos(new_angle, speed), 100);
	
	SpearEffect();
}

func Blocked()
{
	SpearEffect();
}