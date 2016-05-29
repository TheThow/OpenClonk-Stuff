/*-- Big Rock --*/


local ManaCost = 40;
local SpellDamage = 30;
local Speed = 60;
local Charge_dur = 35;

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

func HitByHook(hook)
{
	if(!snapped)
		return;
		
	Unstuck();
	snapped = 0;
	var fx = GetEffect("Travel", this);
	RemoveEffect("Travel", this);
	SetAction("Idle");
	
	/*
	var dir;
	if(GetX() > hook->GetX())
		dir = 1;
	else
		dir = -1;*/
		
	var angle = Angle(hook->GetX(), hook->GetY(), GetX(), GetY());
	
	//var angle = fx.angle - 45 * -fx.dir;
	var xdir = Sin(angle, Speed);
	var ydir = -Cos(angle, Speed);
	//var xdir = Cos(angle, Speed);
	//var ydir = Sin(angle, Speed);
	Bounce(xdir, ydir);
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
	SetVelocity(angle, Speed-10, 10);
	
	Sound("sawblade_launch", false, 100);
	
	AddEffect("CheckEnemies", this, 1,1, this);
	
	SetLightRange(30, 70);
	SetLightColor(RGB(150, 150, 150));
	
	AddEffect("Life", this, 20, LifeTime, this);

	/*if(GetXDir() > 0)
		SetRDir(15);
	else
		SetRDir(-15);*/
	
	SetClrModulation(RGBa(255,255,255,255));
	Sound("sawloop", false, 20, nil, 1);
	
	AddEffect("Rotate", this, 1, 1, this);
}

func FxRotateStart(target, fx, temp)
{
	if(temp)
		return;
	
	fx.dir = 1;
}

func FxRotateTimer(target, fx)
{
	if(this)
		SetR(GetR() + 25 * fx.dir);
}

func FxCheckEnemiesTimer(object target, proplist effect, int time)
{
	for(var o in FindObjects(Find_Distance(Size), Find_Not(Find_ID(Hook)), Find_Or(Find_Func("IsReflectable"), Find_Func("CanBeHit", this))))
	{
		if(GetEffect("SawBladeCD", o) || (o->GetOwner() == GetOwner() && time < 15))
		{
			continue;
		}
		
		var angle = Angle(GetX(), GetY(), o->GetX(), o->GetY());
		AddEffect("SawBladeCD", o, 1, 25);
		Sound("Objects::Weapons::WeaponHit*", false, 50);
		
		if(o->GetID() != Clonk)
		{
			var speed = Distance(0, 0, o->GetXDir(), o->GetYDir());
			o->SetVelocity(angle, speed);
			WeaponDamage(o, SpellDamage);
			o->Blocked(this);
			continue;
		}
		
		o->Fling(Sin(angle, 8), -Cos(angle, 8));
		WeaponDamage(o, SpellDamage);
	}
}


func ChargeEffect(proplist params)
{
	var a = params.new_angle;
	var x = Sin(a, 10, 10);
	var y = -Cos(a, 10, 10);

	CreateParticle("Shockwave2", x, y, PV_Random(-10,10), PV_Random(-15, 5), 20, chargeprt, 1);
}


func Hit(xdir, ydir)
{
	if(!snapped)
	{
		Sound("hooksnap", false, 50, nil, nil, nil, 70);
		var fx = AddEffect("Travel", this, 1, 1, this);
		if(xdir)
			fx.dir = xdir / Abs(xdir);
		else
			fx.dir = 1;
			
		var rfx = GetEffect("Rotate", this);
			rfx.dir = fx.dir;
			
		fx.v = 0;
		fx.angle = 0;
		if(fx.dir == -1)
			fx.angle = 180;
		
		if	(ydir < 0)
			fx.angle -= 45 * fx.dir;
		else if(ydir > 0)
			fx.angle += 45 * fx.dir;
		
		snapped = true;
		SetAction("Travel");
		SetXDir(0);
		SetYDir(0);
		
		if(!GBackSolid())
		{
			if(GBackSolid(-1,0))
				SetPosition(GetX()-1, GetY());
			else if(GBackSolid(0,-1))
				SetPosition(GetX(), GetY()-1);
			else if(GBackSolid(1,0))
				SetPosition(GetX()+1, GetY());
			else
				SetPosition(GetX(), GetY()+1);
		}
	}
}

func FxTravelTimer(target, fx)
{
	var step = 4;
	
	var x = GetX();
	var y = GetY();
	
	//SetR(GetR() + 15 * fx.dir);
	
	for(var i = 0; i < 180; i+=5)
	{
		var nx = Cos(fx.angle + i, step);
		var ny = Sin(fx.angle + i, step);
		
		if(IsValidPixel(nx, ny, step))
		{
			SetPosition(x + nx, y + ny);
			fx.angle = Angle(0, 0, nx, ny) - 90;
			break;
		}
		
		nx = Cos(fx.angle - i, step);
		ny = Sin(fx.angle - i, step);
		
		if(IsValidPixel(nx, ny, step))
		{
			SetPosition(x + nx, y + ny);
			fx.angle = Angle(0, 0, nx, ny) - 90;
			break;
		}
	}
	
	if(!GBackSolid())
	{
		snapped = 0;
		SetAction("Idle");
		return -1;
	}
	
	
	/*
	if(GBackSolid(0, step) && !GBackSolid(step * fx.dir, 0))
	{
		SetPosition(x + step * fx.dir, y);
		fx.v = +1;
		return;
	}
	
	if(GBackSolid(0, -step) && !GBackSolid(step * -fx.dir, 0))
	{
		SetPosition(x + step * -fx.dir, y);
		return;
	}
	
	if(GBackSolid(0, step) && !GBackSolid(0, -step) && GBackSolid(step * fx.dir, 0))
	{
		SetPosition(x, y - step);
		fx.v = -1;
		return;
	}
	
	if(!GBackSolid(0, step) && GBackSolid(0, -step) && GBackSolid(step * -fx.dir, 0))
	{
		SetPosition(x, y + step);
		return;
	}
	
	if(CheckCrossFree(step) && GBackSolid(step*2 * -fx.dir, step*2))
	{
		var c = 0;
		
		//if(!GBackSolid(2 * -fx.diir, step*2));
		//	c = 2 * -fx.dir;
		
		SetPosition(x + c, y + step);
		fx.v = 1;
		return;
	}
	
	if(CheckCrossFree(step) && GBackSolid(step*2 * fx.dir, -step*2))
	{
		var c = 0;
		
		//if(!GBackSolid(2 * fx.diir, -step*2));
		//	c = 2 * fx.dir;
		
		SetPosition(x + c, y - step);
		fx.v = -1;
		return;
	}
	
	if(CheckCrossFree(step) && GBackSolid(step*2 * -fx.dir, -step*2))
	{
		var c = 0;
		
		//if(!GBackSolid(step*2 * -fx.dir, -2));
		//	c = -2;
		
		SetPosition(x + step * -fx.dir, y + c);
		fx.v = 0;
		return;
	}
	
	if(CheckCrossFree(step) && GBackSolid(step*2 * fx.dir, step*2))
	{
		var c = 0;
		
		//if(!GBackSolid(step*2 * fx.dir, 2));
		//	c = 2;
		
		SetPosition(x + step * fx.dir, y + c);
		fx.v = 0;
		return;
	}
	
	if(fx.v != 0 && GBackSolid(step * -fx.dir * fx.v, 0))
	{
		SetPosition(x, y + step * fx.v);
		return;
	}
	else
		fx.v = 0;*/
}

func CheckCrossFree(int step)
{
	return !GBackSolid(-step, 0) && !GBackSolid(step, 0) && !GBackSolid(0, step) && !GBackSolid(0, -step);
}

func IsValidPixel(x, y, step)
{
	step=2;
	return GBackSolid(x,y) && (!GBackSolid(x+step,y) || !GBackSolid(x-step, y) || !GBackSolid(x, y+step) || !GBackSolid(x, y-step));
}


func FxLifeStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;
	
	var prt = {
		Size = 15,
		Alpha = PV_Linear(255, 0),
		Rotation = PV_Step(15, PV_Random(0, 360)),
		ForceY = GetGravity()
	};
	CreateParticle("SawbladePrt", 0, 0, PV_Random(-35, 35), PV_Random(-35, 5), 40, prt, 6);
	
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
	
	CreateParticle("Flash", 0, 0, PV_Random(-35, 35), PV_Random(-35, 5), 70, prt2, 20);
	
	Sound("sawblade_destroyed", false, 50);
	RemoveObject();
}

func Bounce(int xdir, int ydir)
{
	var angle = Angle(0, 0, xdir, ydir);
	
	var surface = GetSurfaceVector(0, 0);
	var surface_angle = Angle(0, 0, surface[0], surface[1]);
	var angle_diff = GetTurnDirection(angle - 180, surface_angle);
	var new_angle = surface_angle + angle_diff;
	
	SetXDir(Sin(new_angle, Speed*10), 100);
	SetYDir(-Cos(new_angle, Speed*10), 100);
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
		Speed=0
	},
};
