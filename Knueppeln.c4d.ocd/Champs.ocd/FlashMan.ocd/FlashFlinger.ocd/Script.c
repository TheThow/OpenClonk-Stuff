#include Projectile

local Speed = 95;
local SpellDamage = 22;
local Size = 30;
local ManaCost = 20;

local hitspots;

//local MaxTravelTime = 180;

local TravelAngle = 0;

func Initialize()
{
	hitspots = [];
}

func InitEffect()
{
	Sound("Flash::projectileshoot", false, 100);
}

func TravelEffect(int time)
{
	var trailparticles =
	{
		Size = PV_Linear(PV_Random(10, 22),10),
		BlitMode = GFX_BLIT_Additive,
		//R = 250 - (250 * time) / MaxTravelTime,
		R = PV_Random(20, 180),
		G = PV_Random(20, 180),
		//B = (250 * time) / MaxTravelTime,
		B = PV_Random(20, 180),
		Alpha = PV_Linear(255,0),
	};
	
	CreateParticle("Flash", PV_Random(-2,2), PV_Random(-2,2), 0, 0, 12, trailparticles);
	
	if (time > 35)
	{
		DoTheLasers();
		RemoveObject();
	}
}

func DoTheLasers()
{
	var numlasers = GetLength(hitspots);
	
	if (numlasers == 0)
	{
		Sound("Flash::spectralhit", nil, nil, nil, nil, nil, -60);
		return;
	}
	
	SetCategory(C4D_StaticBack);
	SetXDir(0);
	SetYDir(0);
	
	var posx = GetX();
	var posy = GetY();
	
	var counter = 0;
	
	var dmgperlaser;
	
	for (var spot in hitspots)
	{
		var launcher = CreateObject(FlashLaserLauncher, spot[0] - posx, spot[1] - posy);
		launcher->LazorTo(posx - launcher->GetX(), posy - launcher->GetY(), counter * 10, SpellDamage / numlasers);
		
		counter++;
	}
}

func HitObject(obj)
{
	if(obj->~CanBeHit(this) == false)
		return;
		
	obj->Fling(0, -2, nil, true);
	WeaponDamage(obj, 1);
}

func HitDamage(obj)
{
	if(obj->~CanBeHit(this) == false)
		return;
	
	AddEffect("FlashHit", obj, 20, 1, nil, FlashMan);
	WeaponDamage(obj, 1);
}

func Hit(int xdir, int ydir)
{
	PushBack(hitspots, [GetX(), GetY()] );
	
	Sound("Flash::Glass?");
	Bounce(xdir, ydir);
}

func Bounce(int xdir, int ydir)
{
	var angle = Angle(0, 0, xdir, ydir);
	
	var surface = GetSurfaceVector(0, 0);
	var surface_angle = Angle(0, 0, surface[0], surface[1]);
	var angle_diff = GetTurnDirection(angle - 180, surface_angle);
	var new_angle = surface_angle + angle_diff;
	
	var speed = Distance(0, 0, xdir, ydir);
	//speed = speed*3/4;
	SetXDir(Sin(new_angle, speed), 100);
	SetYDir(-Cos(new_angle, speed), 100);
}

