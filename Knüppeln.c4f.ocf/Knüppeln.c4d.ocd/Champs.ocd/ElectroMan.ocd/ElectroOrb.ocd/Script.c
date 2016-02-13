/**
	ElectroOrb
	

	@author 
*/

local ManaCost = 30;
local SpellDamage = 20;
local Speed = 55;
local Dur = 30;
local Charge_dur = 15;
local l_effect_range = 100;

local Size = 15;

local Name = "$Name$";
local Description = "$Description$";

local LifeTime = 200;

local plr_hit;

local Target;

local pR = 175;
local pG = 215;
local pB = 255;

func Initialize()
{
	SetAction("Travel");
	SetRDir(10);
	SetClrModulation(RGBa(0,0,0,0));
	plr_hit = CreateArray(GetPlayerCount());
}

func Launch(object clonk, int x, int y)
{
	var params = {
		angle = Angle(0,0,x,y),
		cl = clonk
	};
	clonk->Charge(this, "ChargeStop", Charge_dur, params);
	Target = clonk;
}

func ChargeStop(proplist params)
{
	var angle = params.new_angle;
	SetVelocity(angle, Speed, 10);
	
	AddEffect("ElectroOrb", this, 20, 1 ,this, GetID());
	
	Sound("electro_shot", false, 100);
	Sound("electro_travel", false, 20, nil, 1);
	
	OrbEffect();
	AddEffect("HitCheck", this, 1,1, nil,nil, params.cl);
	
	//AddEffect("OrbTravel", this, 20, 1, this, GetID());
	
	SetLightRange(30, 70);
	SetLightColor(RGB(255, 255, 255));
}

func ChargeEffect(proplist params)
{
	var flashparticle2 =
	{
		Alpha = 30,
		Size = Size/3,
		R = pR,
		G = pG,
		B = pB,
		BlitMode = GFX_BLIT_Additive,
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	
	var a = params.new_angle;
	var x = Sin(a, 10, 10);
	var y = -Cos(a, 10, 10);
	
	CreateParticle("Flash", x, y, 0, 0, 5, flashparticle2, 2);
	
	for(var i = a/10 - 90; i < a/10 + 270; i+= RandomX(3,10))
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
			
			x = x + Sin(i, Size/3 + RandomX(-2, 2));
			y = y + -Cos(i, Size/3 + RandomX(-2, 2));
			
			var xdir = Sin(i + 180, RandomX(5, 10));
			var ydir = -Cos(i + 180, RandomX(5, 10));
			
			CreateParticle("Lightning", x, y, xdir, ydir, 5, props, 1);
		}
	}
}
/*
func FxOrbTravelTimer(object target, proplist effect, int time)
{
	var props =
	{
		Size = 5,
		Alpha = PV_KeyFrames(0, 0, 200, 100, 0, 200, 255, 1000, 0),
		Phase = PV_Random(0, 5),
		BlitMode = GFX_BLIT_Additive,
		R = pR,
		G = pG,
		B = pB,
	};

	if(effect.cd > 0)
	{
		effect.cd--;
	}
	else
	{
		var r = Random(360);
		
		for(var i = 0; i < l_effect_range; i++)
		{
			var x = Sin(r, i);
			var y = -Cos(r, i);
		
			if(GBackSolid(x, y))
			{
				DrawLightningSmall(0, 0, x, y, props);
				effect.cd = 20;
				break;
			}
		}
	}
}
*/
func FxElectroOrbTimer(object target, proplist effect, int time)
{
	CheckForEnemies();
	
	if(time == Dur)
	{
		Return();
		return -1;
	}
}

func FxComebackTimer(object target, proplist effect, int time)
{
	if(!Target)
	{
		RemoveObject();
		return -1;
	}

	CheckForEnemies();
	
	var angle = Angle(GetX(), GetY(), Target->GetX(), Target->GetY(), 10);
	
	var txdir = Sin(angle, Speed, 10);
	var tydir = -Cos(angle, Speed, 10);
	
	SetXDir((GetXDir() + (txdir - GetXDir())/10));
	SetYDir((GetYDir() + (tydir - GetYDir())/10));
	
	if(ObjectDistance(this, Target) < Size)
		RemoveObject();
}

func OrbEffect()
{
	var lightparticle =
	{
		Prototype = Particles_ElectroSpark2(),
		Alpha = 255,
		Size = PV_Random(10, 20, 1),
		Rotation = PV_Step(10, PV_Random(0, 360, 5), 1),
		BlitMode = GFX_BLIT_Additive,
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	CreateParticle("ElectroSpark", 0, 0, 0, 0, 0, lightparticle, 20);
	
	var sphereparticle =
	{
		Alpha = 80,
		Size = Size,
		R = pR,
		G = pG,
		B = pB,
		BlitMode = GFX_BLIT_Additive,
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	CreateParticle("Sphere", 0, 0, 0, 0, 0, sphereparticle);
	
	var flashparticle =
	{
		Alpha = 30,
		Size = Size*2,
		R = pR,
		G = pG,
		B = pB,
		BlitMode = GFX_BLIT_Additive,
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	CreateParticle("Flash", 0, 0, 0, 0, 0, flashparticle);
	
	var flashparticle2 =
	{
		Alpha = 30,
		Size = Size/2,
		R = pR,
		G = pG,
		B = pB,
		BlitMode = GFX_BLIT_Additive,
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	CreateParticle("Flash", 0, 0, 0, 0, 0, flashparticle2, 5);
}

func CheckForEnemies()
{
	for(var o in FindObjects(Find_Distance(Size), Find_Func("CanBeHit")))
	{
		if(o->GetOwner() == GetOwner())
			continue;
		
		if(plr_hit[GetPlayerByIndex(o->GetOwner())] == 0 || plr_hit[GetPlayerByIndex(o->GetOwner())] == nil)
		{
			plr_hit[GetPlayerByIndex(o->GetOwner())] = 1;
			o->Fling(0,-1);
			o->AddElectroHitEffect();
			
			Sound("electro_shot", false, 50);
			
			var trailparticles =
			{
				Prototype = Particles_ElectroSpark2(),
				Size = PV_Linear(PV_Random(5,15),0),
				BlitMode = GFX_BLIT_Additive,
				Rotation = PV_Random(0,360),
				R = pR,
				G = pG,
				B = pB,
			};
			
			CreateParticle("Lightning", o->GetX() - GetX(), o->GetY() - GetY(), 0, 0, 10, trailparticles, 5);
			
			WeaponDamage(o, SpellDamage);
		}
	}
}

func Hit(xdir, ydir)
{
	Bounce(xdir, ydir);
	
	if(Distance(0,0, GetXDir(), GetYDir()) > 30)
		Sound("electro_shot", false, 50);
}

func IsReflectable()
{
	return 1;
}

func Return()
{
	RemoveEffect("ElectroOrb", this);
	AddEffect("Remove", this, 20, LifeTime, this, GetID());
	for(var i = 0; i < GetLength(plr_hit); i++)
		plr_hit[i] = 0;
	
	AddEffect("Comeback", this, 20, 1, this, GetID());
}

func FxRemoveStop(object target, proplist effect, int reason, bool temporary)
{
	OnRemove();
	
	RemoveObject();
}

func ChargeInterrupted()
{
	RemoveObject();
}

func OnRemove()
{
	var particles =
	{
		Prototype = Particles_Glimmer(),
		R = pR,
		G = pG,
		B = pB,
		Alpha = 255,
		Size = PV_Linear(5, 0),
	};
	CreateParticle("StarSpark", 0, 0, PV_Random(-50,50), PV_Random(-50, 50), 20, particles, 15);
	
	var sphereparticle =
	{
		Alpha = PV_Linear(255, 0),
		Size = 10,
		R = pR,
		G = pG,
		B = pB,
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("StarSpark", 0, 0, 0, 0, 10, sphereparticle, 4);
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
		StartCall = "Traveling",
		Speed=1000
	},
};

func Bounce(int xdir, int ydir)
{
	var angle = Angle(0, 0, xdir, ydir);
	
	var surface = GetSurfaceVector(0, 0);
	var surface_angle = Angle(0, 0, surface[0], surface[1]);
	var angle_diff = GetTurnDirection(angle - 180, surface_angle);
	var new_angle = surface_angle + angle_diff;
	
	var speed = Distance(0, 0, xdir, ydir);
	speed = speed;
	SetXDir(Sin(new_angle, speed), 100);
	SetYDir(-Cos(new_angle, speed), 100);
}