/*-- Big Rock --*/


local ManaCost = 40;
local SpellDamage = 30;
local Speed = 60;
local Charge_dur = 35;

local Size = 20;
local chargeprt;
local snapped;

func Initialize()
{
	snapped = false;
	AddEffect("CheckEnemies", this, 1,1, this);
	SetAction("Idle");
	
	AddEffect("Rotate", this, 1, 1, this);
}

func Circle(radius, speed, dir, startangle)
{
	var fx = AddEffect("Circle", this, 1, 1, this);
	fx.x = GetX();
	fx.y = GetY();
	fx.speed = speed;
	fx.radius = radius;
	fx.angle = startangle;
	fx.dir = dir;
	snapped = true;
	SetCategory(C4D_StaticBack);
}

func FxCircleTimer(t, fx)
{
	fx.angle += fx.speed * fx.dir;
	SetPosition(fx.x + Cos(fx.angle, fx.radius), fx.y + Sin(fx.angle, fx.radius));
}

func Shizzle(x, y, step, offx, offy)
{
	snapped = true;
	SetCategory(C4D_StaticBack);
	var fx = AddEffect("Shizzle", this, 1, 1, this);
	fx.ox = GetX() + offx;
	fx.oy = GetY() + offy;
	fx.tx = GetX() + x;
	fx.ty = GetY() + y;
	fx.step = step;
	fx.c = 0;
}

func FxShizzleTimer(target, fx)
{
	var angle = Angle(fx.ox, fx.oy, fx.tx, fx.ty) - 90 + 180 * fx.c;
	var x = Cos(angle, fx.step);
	var y = Sin(angle, fx.step);
	SetPosition(GetX() + x, GetY() + y);
	
	if (Distance(fx.ox, fx.oy, GetX(), GetY()) < fx.step && fx.c == 1)
	{
		fx.c = 0;
	}
		if (Distance(fx.tx, fx.ty, GetX(), GetY()) < fx.step && fx.c == 0)
	{
		fx.c = 1;
	}
}

func Stick()
{
	SetCategory(C4D_StaticBack);
}

func Snap(angle)
{
	if(!snapped)
	{
		Sound("hooksnap", false, 50, nil, nil, nil, 70);
		var fx = AddEffect("Travel", this, 1, 1, this);
		fx.dir = 1;
			
		var rfx = GetEffect("Rotate", this);
		rfx.dir = fx.dir;
			
		fx.v = 0;
		fx.angle = angle;
		
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
		
		
		if(!o->GetAlive())
		{
			if(o->~IsWallElement())
				continue;
			
			var speed = Distance(0, 0, o->GetXDir(), o->GetYDir());
			o->SetVelocity(angle, speed);
			o->~Blocked(this);
			WeaponDamage(o, SpellDamage);
			
			Sound("Hits::GeneralHit*", false, 50);
			continue;
		}
		
		Sound("Objects::Weapons::WeaponHit*", false, 50);
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
