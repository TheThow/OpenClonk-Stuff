/**
	ThunderStrike
	

	@author 
*/

local ManaCost = 60;
local SpellDamage = 50;
local SpellRange = 250;

local Delay = 30;
local Size = 30;

local pR = 175;
local pG = 215;
local pB = 255;


func Initialize()
{
	SetAction("Travel");
	SetClrModulation(RGBa(0,0,0,0));
}

func Launch(object clonk, int x, int y)
{
	AddEffect("Charge", this, 20, 1, this, GetID());
	Sound("electro_charge", false, 50);
	
	var flashparticle =
	{
		Alpha = 20,
		Size = Size*2,
		R = pR,
		G = pG,
		B = pB,
		BlitMode = GFX_BLIT_Additive,
		Attach = ATTACH_Back | ATTACH_MoveRelative,
		Rotation = PV_Step(50)
		
	};
	CreateParticle("StarFlash", 0, 0, 0, 0, 0, flashparticle);
}

func FxChargeTimer(object target, proplist effect, int time)
{
	if (time >= Delay)
		return -1;
		
	for(var i = 0; i < 360; i+= RandomX(3,15))
	{
		if(!Random(10))
		{
			var props =
			{
				Size = PV_Linear(5,4),
				R = 255, G = 255, B = 255,
				Alpha = PV_KeyFrames(0, 0, 200, 100, 0, 200, 255, 1000, 0),
				Phase = PV_Random(0, 5),
				BlitMode = GFX_BLIT_Additive,
				Rotation = i + 180,
			};
			
			var x = Sin(i, Size + RandomX(-2, 2));
			var y = -Cos(i, Size + RandomX(-2, 2));
			
			var xdir = Sin(i + 180, RandomX(10, 80));
			var ydir = -Cos(i + 180, RandomX(10, 80));
			
			CreateParticle("Lightning", x, y, xdir, ydir, 5, props, 1);
		}
	}
}

func FxChargeStop()
{
	Sound("electro_explosion", false, 100);
	CheckForEnemies();
	
	var props =
	{
		Size = 10,
		Alpha = PV_KeyFrames(0, 0, 200, 100, 0, 200, 255, 1000, 0),
		Phase = PV_Random(0, 5),
		BlitMode = GFX_BLIT_Additive,
		R = pR,
		G = pG,
		B = pB,
	};
	
	
	DrawLightning(-20, -GetY(), 0, 0, props);
	DrawLightning(0, -GetY(), 0, 0, props);
	DrawLightning(20, -GetY(), 0, 0, props);
	
	var flashparticle =
	{
		Alpha = 100,
		Size = Size * 2,
		R = pR,
		G = pG,
		B = pB,
		Rotation = PV_Random(0,360),
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("StarSpark", 0, 0, 0, 0, 10, flashparticle, 5);
	
	var particles =
	{
		Prototype = Particles_Glimmer(),
		R = pR,
		G = pG,
		B = pB,
		Alpha = 255,
		Size = PV_Linear(10, 0),
		OnCollision = PC_Bounce(),
	};
	CreateParticle("StarSpark", 0, 0, PV_Random(-80,80), PV_Random(-80, 80), 25, particles, 25);
	
	RemoveObject();
}

func CheckForEnemies()
{
	for(var o in FindObjects(Find_Distance(Size), Find_Func("CanBeHit")))
	{
		var angle = Angle(GetX(), GetY(), o->GetX(), o->GetY());
		
		o->Fling(Sin(angle, 8), -Cos(angle, 8));
		o->DoEnergy(-SpellDamage);
		AddElectroHitEffect(o);
	}
}