/*--- Ruby ---*/

local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Plane = 480;
local graphics_index = 0;

local speed = 80;
local thrown;

local monsters;

local ox;
local oy;

local trailparticles;

local probability = 45;

func IsKnueppelItem() { return true; }

func InitEffect()
{
	SetLightRange(30, 70);
	SetLightColor(RGB(255, 0, 0));
	ox=GetX();
	oy=GetY();
	
	trailparticles =
	{
		Size = PV_Linear(5,0),
		BlitMode = GFX_BLIT_Additive,
		R = 255,
		G = 50,
		B = 50,
	};
	
	AddEffect("Trail", this, 1, 1, this);
}

func FxTrailTimer()
{
	TravelEffect();
}

// returns the color of the gem (used for effects)
func GetGemColor()
{
	return RGB(255, 20, 20);
}

func Initialize()
{
	graphics_index = Random(4);
	if (graphics_index) SetGraphics(Format("%d", graphics_index+1));
	
	monsters = [Wipf, Bat, Piranha, Mooq, Puka, Squid];
	
	thrown = false;
	
	return true;
}

protected func ControlUse(object clonk, int iX, int iY)
{
	Exit();
	Collectible = 0;
	var angle = Angle(0, 0, iX, iY);
	SetPosition(clonk->GetX() + Sin(angle, 5), clonk->GetY() + -Cos(angle, 5));
	SetVelocity(angle, speed);
	thrown = true;
	
	SetOwner(clonk->GetOwner());
	InitEffect();
	
	return true;
}

func TravelEffect()
{
	DrawParticleLine("Flash", 0, 0, ox-GetX(), oy-GetY(), 1, 0, 0, 10, trailparticles);
	ox=GetX();
	oy=GetY();
}

func Hit()
{
	Sound("Hits::Materials::Glass::GlassHit*");
	
	if(!thrown)
		return;
	
	CreateObject(monsters[Random(GetLength(monsters))], 0, - 5, GetOwner());
	
	var flashparticle =
	{
		Alpha = PV_Linear(255,0),
		Size = 30,
		R = 255,
		G = 50,
		B = 50,
		Rotation = PV_Random(0,360),
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("StarSpark", 0, 0, 0, 0, 10, flashparticle, 5);
	
	var particles =
	{
		Prototype = Particles_Glimmer(),
		R = 255,
		G = 50,
		B = 50,
		Alpha = 255,
		Size = PV_Linear(5, 0),
		OnCollision = PC_Bounce(),
	};
	CreateParticle("StarSpark", 0, 0, PV_Random(-80,80), PV_Random(-80, 80), 80, particles, 35);
	
	RemoveObject();
	
	return true;
}