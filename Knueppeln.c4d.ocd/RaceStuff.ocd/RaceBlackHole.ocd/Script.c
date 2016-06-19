/*-- Ore --*/


local Name = "$Name$";
local Description = "$Description$";
local Plane = 900;

local speed = 80;
local thrown;

local ox;
local oy;

local trailparticles;
local flyparticles;

local probability = 20;

local MaxSize = 50;
local size = 0;
local MaxRange = 300;
local Strength;

func Initialize()
{
		
	MaxSize = 50;
	size = 0;
	MaxRange = 300;
	Strength = 10;
		
	trailparticles =
	{
		Size = PV_Linear(8,0),
		BlitMode = GFX_BLIT_Additive,
		R = 255,
		G = 255,
		B = 255,
		Rotation=PV_Random(0, 360),
	};
	
	flyparticles =
	{
		Size = PV_Random(2, 4),
		Alpha = PV_Linear(255,0),
		BlitMode = GFX_BLIT_Additive,
		R = 100,
		G = 0,
		B = 100,
		Stretch = PV_Speed(2000, 1000),
		Rotation = PV_Direction(),
	};
}


func TravelEffect()
{
	DrawParticleLine("StarSpark", 0, 0, ox-GetX(), oy-GetY(), 1, 0, 0, 10, trailparticles);
	ox=GetX();
	oy=GetY();
}



func StartBlackHole(int Size, int Range, int strength)
{
	if(MaxSize != nil)
		MaxSize = Size;
	if(MaxRange != nil)
		MaxRange = Range;
	if(Strength != nil)
		Strength = strength;

	Sound("Items::blackhole", false, 50);
	AddEffect("Grow", this, 1, 1, this);
	AddEffect("Suck", this, 1, 1, this);
}

func FxSuckTimer(target, fx, time)
{
	//ShakeViewport(size/12, 0, 0, MaxRange);
	
	var r = Random(360);
	var x = Cos(r, MaxRange);
	var y = Sin(r, MaxRange);
	
	var angle = Angle(0, 0, x, y);
	CreateParticle("Flash", x, y, Sin(angle + 180, 100), -Cos(angle + 180, 100), 30, flyparticles, 1);
	
	
	for(var obj in FindObjects(Find_Not(Find_Func("CannotBeSucked")), Find_NoContainer(), Find_Distance(MaxRange), Find_Or(Find_Category(C4D_Living), Find_Category(C4D_Object), Find_Func("CanBeSucked"))))
	{
		var dist = Distance(GetX(), GetY(), obj->GetX(), obj->GetY());
		var strength = ((MaxRange)/(dist+1) + 1) * Strength;
		var angle = Angle(GetX(), GetY(), obj->GetX(), obj->GetY()) - 180;
		
		if(obj->GetID() == Clonk && (obj->GetAction() == "Walk" || obj->GetAction() == "Scale"))
		{
			obj->SetAction("Jump");
			obj->SetPosition(obj->GetX(), obj->GetY() - 2);
		}
		if(obj->GetID() == Clonk && obj->GetAction() == "Hangle")
		{
			obj->SetAction("Jump");
		}
		
		obj->AddVelocity(angle, strength, nil, 100);
	
		if(dist < size/2)
		{
			if (obj->GetAlive())
			{
				WeaponDamage(obj, 9001);
			}
			if(obj)
				obj->RemoveObject();
		}
	}
}

func FxGrowTimer(target, fx, time)
{
	size = time;
	var black =
	{
		Alpha = 255,
		Size = size,
		R = 0,
		G = 0,
		B = 0,
		Attach = ATTACH_Front | ATTACH_MoveRelative
	};
	var purple =
	{
		Alpha = 255,
		Size = size + 10,
		R = 100,
		G = 0,
		B = 100,
		Attach = ATTACH_Back | ATTACH_MoveRelative
	};
	if(time < MaxSize)
	{
		CreateParticle("Flash", 0, 0, 0, 0, 1, purple, 1);
		CreateParticle("Flash", 0, 0, 0, 0, 1, black, 2);
	}
	else
	{
		CreateParticle("Flash", 0, 0, 0, 0, 0, purple, 1);
		CreateParticle("Flash", 0, 0, 0, 0, 0, black, 2);
		return -1;
	}
}
