/*-- Big Rock --*/


local ManaCost = 40;
local SpellDamage = 35;
local Speed = 60;
local Charge_dur = 30;

local Size = 20;

local LifeTime = 400;
local chargeprt;
local snapped;

func Initialize()
{
	chargeprt =
	{
		Size = 5,
		Alpha = PV_Linear(255, 0),
		R = 150,
		G = 150,
		B = 150,
		ForceY = GetGravity(),
		Stretch = PV_Linear(1500, 1500),
		Rotation = PV_Step(10, PV_Random(0, 360), 1),
		OnCollision = PC_Bounce(),
	};
	snapped = false;
}


func Launch(object clonk, int x, int y)
{
	var params = {
		angle = Angle(0,0,x,y),
		cl = clonk
	};
	clonk->Charge(this, "ChargeStop", Charge_dur, params);
	SetAction("Travel");
	SetClrModulation(RGBa(255,255,255,0));
}

func ChargeInterrupted()
{
	RemoveObject();
}

func ChargeStop(proplist params)
{
	SetAction("Idle");
	var angle = params.new_angle;
	SetVelocity(angle, Speed, 10);
	
	Sound("Fire::BlastLiquid3", false, 100);
	
	AddEffect("CheckEnemies", this, 20,1, this);
	
	SetLightRange(30, 70);
	SetLightColor(RGB(150, 150, 150));
	
	AddEffect("Life", this, 20, LifeTime, this);

	if(GetXDir() > 0)
		SetRDir(15);
	else
		SetRDir(-15);
	
	SetClrModulation(RGBa(255,255,255,255));
}

func FxCheckEnemiesTimer(object target, proplist effect, int time)
{
	for(var o in FindObjects(Find_Distance(20), Find_Or(Find_Func("IsReflectable"), Find_Func("CanBeHit", this))))
	{
		if(GetEffect("SawBladeCD", o) || (o->GetOwner() == GetOwner() && time < 15))
		{
			continue;
		}
		
		var angle = Angle(GetX(), GetY(), o->GetX(), o->GetY());
		AddEffect("SawBladeCD", o, 1, 15);
		Sound("Objects::Weapons::WeaponHit*", false, 50);
		
		if(o->GetID() != Clonk)
		{
			WeaponDamage(o, SpellDamage);
			var speed = Distance(0, 0, o->GetXDir(), o->GetYDir());
			o->SetVelocity(angle, speed);
			continue;
		}
		
		o->Fling(Sin(angle, 5), -Cos(angle, 5));
		WeaponDamage(o, SpellDamage);
	}
}


func ChargeEffect(proplist params)
{
	CreateParticle("Shockwave2", 0, 0, PV_Random(-10,10), PV_Random(-15, 5), 20, chargeprt, 1);
}


func Hit(xdir, ydir)
{
	if(!snapped)
	{
		Sound("hooksnap", false, 50, nil, nil, nil, 70);
		var fx = AddEffect("Travel", this, 1, 1, this);
		fx.dir = GetXDir() / Abs(GetXDir());
		fx.v = 0;
		snapped = true;
		SetAction("Travel");
		SetXDir(0);
		SetYDir(0);
		RemoveVertex(0);
		
		if(xdir > 0)
			SetRDir(15);
		else
			SetRDir(-15);
	}
}

func FxTravelTimer(target, fx)
{
	var step = 4;
	
	var x = GetX();
	var y = GetY();
	
	if(!GBackSolid(0, step) && (GBackSolid(step, 0) || GBackSolid(-step, 0)))
		fx.v = 1;
	else
		fx.v = 0;
	
	if(!GBackSolid(0, -step) && (GBackSolid(step, 0) || GBackSolid(-step, 0)))
		fx.v = -1;
	else
		fx.v = 0;
	
	if(GBackSolid(step*2 * -fx.dir, step*2) && CheckCrossFree(step))
		fx.v = 1;
	
	if(GBackSolid(step * fx.dir, 0) && GBackSolid(0, step))
	{
		fx.v = -1;
	}
	
	if(GBackSolid(step * fx.dir, 0) && GBackSolid(0, -step))
	{
		fx.v = +1;
	}
	
	if(fx.v)
	{
		y += step * fx.v;
	}
	
	if((!GBackSolid(step * fx.dir, 0) && GBackSolid(0, step)) && fx.v == 0)
		x += step * fx.dir;
	
	if((!GBackSolid(step * -fx.dir, 0) && GBackSolid(0, -step)) && fx.v == 0)
		x -= step * fx.dir;
	
	
	
	SetPosition(x,y);
}

func CheckCrossFree(int step)
{
	return !GBackSolid(-step, 0) && !GBackSolid(step, 0) && !GBackSolid(0, step) && !GBackSolid(0, -step);
}


func FxLifeStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;
		
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
		Speed=1000
	},
};
