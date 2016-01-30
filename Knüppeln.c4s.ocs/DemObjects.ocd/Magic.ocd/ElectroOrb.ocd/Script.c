/**
	ElectroOrb
	

	@author 
*/

local ManaCost = 40;
local SpellDamage = 20;
local Speed = 40;
local Durr = 35;
local Charge_durr = 20;

local Size = 15;

local Name = "$Name$";
local Description = "$Description$";

local LifeTime = 200;

local plr_hit;

local Target;

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
	
	Sound("electric_shot", false, 100);
	Sound("electro_travel", false, 20, nil, 1);
	
	OrbEffect();
	AddEffect("HitCheck", this, 1,1, nil,nil, params.cl);
	
	SetLightRange(30, 70);
	SetLightColor(RGB(255, 255, 255));
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
		R = 150,
		G = 200,
		B = 255,
		BlitMode = GFX_BLIT_Additive,
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	CreateParticle("Sphere", 0, 0, 0, 0, 0, sphereparticle);
	
	var flashparticle =
	{
		Alpha = 30,
		Size = Size*2,
		R = 150,
		G = 200,
		B = 255,
		BlitMode = GFX_BLIT_Additive,
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	CreateParticle("Flash", 0, 0, 0, 0, 0, flashparticle);
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
			Sound("electric_shot", false, 50);
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
		R = 150,
		G = 200,
		B = 255,
		Alpha = 255,
		Size = PV_Linear(5, 0),
	};
	CreateParticle("StarSpark", 0, 0, PV_Random(-50,50), PV_Random(-50, 50), 20, particles, 15);
	
	var sphereparticle =
	{
		Alpha = PV_Linear(255, 0),
		Size = 10,
		R = 150,
		G = 200,
		B = 255,
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