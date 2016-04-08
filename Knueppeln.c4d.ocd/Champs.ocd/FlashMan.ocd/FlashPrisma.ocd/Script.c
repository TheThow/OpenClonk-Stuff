#include Projectile

local Speed = 90;
local SpellDamage = 33;
local Size = 20;
local ManaCost = 20;

local traveledTime = 0;
local MaxTravelTime = 25;

local LaserLength = 60;

local TravelAngle = 0;

func Initialize()
{
	
}

func InitEffect()
{
	Sound("Flash::projectileshoot", false, 100);
}

func TravelEffect(int time)
{
	var trailparticles =
	{
		Size = PV_Linear(PV_Random(7, 12),0),
		BlitMode = GFX_BLIT_Additive,
		R = 250 - (250 * time) / MaxTravelTime,
		G = 50,
		B = (250 * time) / MaxTravelTime,
	};
	
	CreateParticle("Flash", PV_Random(-2,2), PV_Random(-2,2), 0, 0, 10, trailparticles);
	
	if (time > MaxTravelTime)
	{
		TravelAngle = Angle(0,0, GetXDir(), GetYDir());
		
		FireDaLaser(TravelAngle, 0);
		FireDaLaser(TravelAngle + 120, 1);
		FireDaLaser(TravelAngle - 120, 2);
		
		Sound("Flash::lighthit", false, 100);
		
		RemoveObject();
	}
}

func HitObject(obj)
{
	if(obj->~CanBeHit() == false)
		return;
		
	obj->Fling(0, -2, nil, true);
}

func HitDamage(obj)
{
	if(obj->~CanBeHit() == false)
		return;
	
	obj->AddFlashHitEffect();
	WeaponDamage(obj, SpellDamage);
}

func Hit(int xdir, int ydir)
{
	Bounce(xdir, ydir);
	Sound("Flash::Glass?");
}

func Bounce(int xdir, int ydir)
{
	var angle = Angle(0, 0, xdir, ydir);
	
	var surface = GetSurfaceVector(0, 0);
	var surface_angle = Angle(0, 0, surface[0], surface[1]);
	var angle_diff = GetTurnDirection(angle - 180, surface_angle);
	var new_angle = surface_angle + angle_diff;
	
	var speed = Distance(0, 0, xdir, ydir);
	speed = speed*3/4;
	SetXDir(Sin(new_angle, speed), 100);
	SetYDir(-Cos(new_angle, speed), 100);
}

func FireDaLaser(int angle, int color)
{
	// Color: 0 = red, 1 = green, 2 = blue
	
	var from_x = 0;
	var from_y = 0;
	var to_x   = from_x + Sin(angle, LaserLength);
	var to_y   = from_y - Cos(angle, LaserLength);
	
	for (var i = 0; i <= LaserLength; i += 5)
	{
		var posx = Sin(angle, i);
		var posy = Cos(angle, i) * -1;
		
		var lazorflash = 
		{
			Prototype = Particles_ElectroSpark2(),
			Size = PV_Linear( (15 * i) / LaserLength ,0),
			Rotation = angle,
			R = (color == 0) * 200,
			G = (color == 1) * 200,
			B = (color == 2) * 200,
			Alpha = PV_Linear(100,0),
			OnCollision = PC_Bounce(500),
		};
		
		CreateParticle("Lightning", posx, posy, 0, 0, 40, lazorflash, 10);
		if (i % 20 == 0) CreateLight(posx, posy, 50, 1, nil, 0, 40);
	}
	
	
	for (var obj in FindObjects(Find_OnLine(from_x, from_y, to_x, to_y), Find_OCF(OCF_Alive), Find_Exclude(this.shooter), Find_Func("CanBeHit")))
	{
		this->HitDamage(obj);
	}
	
}

