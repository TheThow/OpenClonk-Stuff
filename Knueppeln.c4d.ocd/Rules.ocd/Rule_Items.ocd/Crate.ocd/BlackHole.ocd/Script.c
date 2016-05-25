/*-- Ore --*/

local probability = 20;

func IsKnueppelItem() { return true; }

local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Plane = 900;

local speed = 80;
local thrown;

local ox;
local oy;

local trailparticles;
local flyparticles;

local probability = 15;

local MaxSize = 100;
local size = 0;
local MaxRange = 500;
local Duration = 240;

func Initialize()
{
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

func InitEffect()
{
	SetLightRange(30, 70);
	SetLightColor(RGB(255, 0, 0));
	ox=GetX();
	oy=GetY();
	
	AddEffect("Trail", this, 1, 1, this);

}

func FxTrailTimer(target, fx, time)
{
	TravelEffect();
	
	if(time > 35)
	{
		Hit();
		return -1;
	}
}

// returns the color of the gem (used for effects)
func GetGemColor()
{
	return RGB(255, 20, 20);
}

protected func Construction()
{
	var graphic = Random(5);
	if(graphic)
		SetGraphics(Format("%d",graphic));
	
	thrown = false;
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
	DrawParticleLine("StarSpark", 0, 0, ox-GetX(), oy-GetY(), 1, 0, 0, 10, trailparticles);
	ox=GetX();
	oy=GetY();
}

func Hit()
{
	
	
	if(!thrown)
	{
		Sound("Hits::Materials::Glass::GlassHit*", false, 50);
		return;
	}
	
	Collectible = 0;
	SetCategory(C4D_StaticBack);
	RemoveEffect("Trail", this);
	Sound("blackhole_explode", false, 50);
	
	var flashparticle =
	{
		Alpha = PV_Linear(255,0),
		Size = PV_Linear(50, 0),
		R = 255,
		G = 255,
		B = 255,
		Rotation = PV_Random(0,360),
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("StarSpark", 0, 0, 0, 0, 50, flashparticle, 5);
	
	var prop2 =
	{
		Alpha = PV_Linear(255,0),
		Size = PV_Linear(1, 0),
		R = 255,
		G = 255,
		B = 255,
		Rotation = PV_Random(0,360),
		BlitMode = GFX_BLIT_Additive,
	};
	
	var r = Random(360);
	var d = 300;
	DrawParticleLine("Flash", Cos(r, d), Sin(r, d), Cos(r + 180, d), Sin(r + 180, d), 1, 0, 0, 50, prop2);
	DrawParticleLine("Flash", Cos(r+90, d), Sin(r+90, d), Cos(r-90, d), Sin(r-90, d), 1, 0, 0, 50, prop2);
	
	ScheduleCall(this, "StartBlackHole", 20, 0);
	
	return true;
}

func StartBlackHole()
{
	AddEffect("Grow", this, 1, 1, this);
	AddEffect("Suck", this, 1, 1, this);
}

func FxSuckTimer(target, fx, time)
{
	ShakeViewport(size/12, 0, 0, MaxRange);
	
	var r = Random(360);
	var x = Cos(r, MaxRange);
	var y = Sin(r, MaxRange);
	
	var angle = Angle(0, 0, x, y);
	CreateParticle("Flash", x, y, Sin(angle + 180, 150), -Cos(angle + 180, 150), 30, flyparticles, 1);
	
	
	for(var obj in FindObjects(Find_Distance(MaxRange), Find_Or(Find_Category(C4D_Living), Find_Category(C4D_Object), Find_Category(C4D_Vehicle))))
	{
		var dist = Distance(GetX(), GetY(), obj->GetX(), obj->GetY());
		var strength = MaxRange/(dist+1) * 12 / 10;
		var angle = Angle(GetX(), GetY(), obj->GetX(), obj->GetY()) - 180;
		
		if(obj->GetID() == Clonk && (obj->GetAction() == "Walk" || obj->GetAction() == "Scale"))
		{
			obj->SetAction("Jump");
			obj->SetPosition(obj->GetX(), obj->GetY() - 1);
		}
		
		obj->AddVelocity(angle, strength);
	
		if(dist < size/2)
		{
			if (obj->GetAlive())
			{
				WeaponDamage(obj, 9001);
			}
			obj->RemoveObject();
		}
	}

	if (time > Duration)
	{
		var black =
		{
			Alpha = PV_Linear(255, 0),
			Size = PV_Linear(size, 0),
			R = 0,
			G = 0,
			B = 0,
		};
		CreateParticle("Flash", 0, 0, 0, 0, 10, black, 2);
		RemoveObject();
		return -1;
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
