/**
	ElectroOrb
	

	@author 
*/

local ManaCost = 35;
local SpellDamage = 20;
local Speed = 40;
local Durr = 35;
local Charge_durr = 20;
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
	clonk->Charge(this, "ChargeStop", Charge_durr, params);
	Target = clonk;
}

func ChargeStop(proplist params)
{
	var angle = params.new_angle;
	SetVelocity(angle, Speed);
	
	AddEffect("ElectroOrb", this, 20, 1 ,this, GetID());
	
	Sound("electro_shot", false, 100);
	Sound("electro_travel", false, 20, nil, 1);
	
	OrbEffect();
	AddEffect("HitCheck", this, 1,1, nil,nil, params.cl);
	
	//AddEffect("OrbTravel", this, 20, 1, this, GetID());
	
	SetLightRange(30, 70);
	SetLightColor(RGB(255, 255, 255));
}

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

func FxElectroOrbTimer(object target, proplist effect, int time)
{
	CheckForEnemies();
	
	if(time == Durr)
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
	
	var angle = Angle(GetX(), GetY(), Target->GetX(), Target->GetY());
	
	var txdir = Sin(angle, Speed);
	var tydir = -Cos(angle, Speed);
	
	SetXDir(GetXDir() + (txdir - GetXDir())/10);
	SetYDir(GetYDir() + (tydir - GetYDir())/10);
	
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
	CreateParticle("Flash", 0, 0, 0, 0, 0, flashparticle2,5);
}

func CheckForEnemies()
{
	for(var o in FindObjects(Find_Distance(Size), Find_ID(Clonk), Find_NoContainer()))
	{
		if(o->GetOwner() == GetOwner())
			continue;
		
		if(plr_hit[GetPlayerByIndex(o->GetOwner())] == 0 || plr_hit[GetPlayerByIndex(o->GetOwner())] == nil)
		{
			plr_hit[GetPlayerByIndex(o->GetOwner())] = 1;
			o->Fling(0,-1);
			o->DoEnergy(-SpellDamage);
			Sound("electro_shot", false, 50);
			AddElectroHitEffect(o);
		}
	}
}

func Hit()
{
	if(!GetEffect("Comeback", this))
		Return();
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