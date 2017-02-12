#include Projectile

local pR = 200;
local pG = 100;
local pB = 0;
local Speed = 70;
local SpellDamage = 25;
local Size = 10;
local ManaCost = 25;
local LifeTime = 45;
local Range = 310;

local trailparticles;
local hit;

local startx;
local starty;
local oldx;
local oldy;

local dummy;

local arr;

func CanBeSucked() { return true; }

func InitEffect()
{
	Sound("Objects::Weapons::WeaponSwing1", false, 100);
	SetLightRange(30, 70);
	SetLightColor(RGB(150, 150, 150));

	var start =
	{
		Size = 10,
		R = pR,
		G = pG,
		B = pB,
		Attach=ATTACH_Front
	};

	var angle = Angle(0, 0, GetXDir(), GetYDir());

	trailparticles =
	{
		Size = 5,
		R = pR,
		G = pG,
		B = pB,
		Stretch = PV_Linear(1500, 1500),
		Rotation = angle,
		Attach=ATTACH_Front
	};

	var hookprt = 
	{
		Size = 8,
		R = 255,
		G = 60,
		B = 0,
		Attach=ATTACH_Front|ATTACH_MoveRelative,
		Rotation = angle
	};
	
	CreateParticle("Hook", GetXDir()/15, GetYDir()/15, 0, 0, 0, hookprt, 1);
	AddEffect("Life", this, 1, LifeTime, this);
	hit = false;
	
	oldx = startx = GetX();
	oldy = starty = GetY();
	
	dummy = CreateObject(Dummy, -GetX(), -GetY(), GetOwner());
	dummy.Visibility = VIS_All;
	dummy->CreateParticle("Shockwave2", GetX(), GetY(), 0, 0, 0, start, 1);
	
	arr = [];
}

func FxLifeStop()
{
	if(!this)
		return;
	Destroy();
}

func TravelEffect(int time)
{
	if(!shooter || !dummy)
		return Destroy();
	
	//CreateParticle("Shockwave2", 0, 0, 0, 0, 0, trailparticles, 1);
	
	//dummy->DrawParticleLine("Shockwave2", GetX(), GetY(), oldx , oldy, 5, 0, 0, 0, trailparticles);
	dummy->CreateParticle("Shockwave2", GetX(), GetY(), 0 , 0, 0, trailparticles, 1);
	oldx = GetX();
	oldy = GetY();
	PushBack(arr, [GetX(), GetY()]);
}

public func HitObject(obj)
{
	if(obj->~CanBeHit(this) == false)
		return;

	dummy->RemoveObject();
	DestroyEffect();
	Explode(SpellDamage);
}

func DestroyEffect()
{
	Sound("Hits::Materials::Metal::LightMetalHit1", false, 50);

	var destroyprt =
	{
		Size = 5,
		Alpha = PV_Linear(255, 0),
		R = pR,
		G = pG,
		B = pB,
		ForceY = GetGravity(),
		Stretch = PV_Linear(1500, 1500),
		Rotation = PV_Step(10, PV_Random(0, 360), 1),
		OnCollision = PC_Bounce(),
	};

	for(var pos in arr)
	{
		CreateParticle("Shockwave2", pos[0] - GetX(), pos[1] - GetY(), PV_Random(-5 ,5), PV_Random(-15, 5), 20, destroyprt, 1); 
	}
}


func Destroy()
{
	if(!this)
		return;
	if(dummy)
		dummy->RemoveObject();
	DestroyEffect();
	
	RemoveObject();
}

func IsReflectable() { return true; }

func Hit(xdir, ydir)
{
	Bounce(xdir, ydir);
	ClearParticles();
	var angle = Angle(0, 0, GetXDir(), GetYDir());
	var hookprt = 
	{
		Size = 8,
		R = 255,
		G = 60,
		B = 0,
		Attach=ATTACH_Front|ATTACH_MoveRelative,
		Rotation = angle
	};
	
		trailparticles =
	{
		Size = 5,
		R = pR,
		G = pG,
		B = pB,
		Stretch = PV_Linear(1500, 1500),
		Rotation = angle,
		Attach=ATTACH_Front
	};
	
	CreateParticle("Hook", GetXDir()/15, GetYDir()/15, 0, 0, 0, hookprt, 1);
	Sound("UI::Ding", false, 100, nil, nil, nil, 500);
}

func Blocked()
{
	ClearParticles();
	var angle = Angle(0, 0, GetXDir(), GetYDir());
	var hookprt = 
	{
		Size = 8,
		R = pR,
		G = pG,
		B = pB,
		Attach=ATTACH_Front|ATTACH_MoveRelative,
		Rotation = angle
	};
	CreateParticle("Hook", GetXDir()/15, GetYDir()/15, 0, 0, 0, hookprt, 1);
	
		trailparticles =
	{
		Size = 5,
		R = pR,
		G = pG,
		B = pB,
		Stretch = PV_Linear(1500, 1500),
		Rotation = angle,
		Attach=ATTACH_Front
	};
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