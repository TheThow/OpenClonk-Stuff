
local Speed = 50;
local Type = "none";

local ManaCost = 25;
local SpellDamage = 15;

func Initialize()
{
	SetAction("Travel");
	SetRDir(10);
	SetClrModulation(RGBa(0,0,0,0));
}

func IsReflectable()
{
	return true;
}

func Launch(object clonk, int x, int y)
{
	Type = clonk->GetChampType();
	var angle = Angle(0,0,x,y);

	AddEffect("HitCheck", this, 1,1, nil,nil, clonk);
	AddEffect("TheEffect", this, 20, 1, this, Projectile);

	SetVelocity(angle, Speed);
	
	/*
	if (Type == "Electro")
	{
		Sound("electric_shot", false, 100);
		Sound("electro_travel", false, 50, nil, 1);
		SetLightRange(30, 70);
		SetLightColor(RGB(255, 255, 255));
	}
	
	if (Type == "Fire")
	{
		Sound("Fire::Fireball", false, 100);
		Sound("Fire::FuseLoop", false, 20, nil, 1);
		SetLightRange(30, 70);
		SetLightColor(RGB(255, 100, 0));
	}*/
}

func FxTheEffectStart()
{

	this->~InitEffect();
/*
	if(Type == "Electro")
	{	
		var lightparticle =
		{
			Alpha = 30,
			Size = 30,
			BlitMode = GFX_BLIT_Additive,
			Rotation = PV_Step(30, 0, 1),
			Attach = ATTACH_Back | ATTACH_MoveRelative
			
		};
		CreateParticle("Flash", 0, 0, 0, 0, 0, lightparticle);
	}

	if(Type == "Fire")
	{
		var lightparticle =
		{
			R = 255,
			G = 100,
			B = 0,
			Alpha = 40,
			Size = 50,
			BlitMode = GFX_BLIT_Additive,
			Rotation = PV_Step(30, 0, 1),
			Attach = ATTACH_Back | ATTACH_MoveRelative
			
		};
		CreateParticle("Shockwave", 0, 0, 0, 0, 0, lightparticle);
		
		
	}*/
}

func FxTheEffectTimer(object target, proplist effect, int time)
{
/*
	if (Type == "Electro")
	{
		var trailparticles =
		{
			Prototype = Particles_ElectroSpark2(),
			Size = PV_Linear(RandomX(5,15),0),
			BlitMode = GFX_BLIT_Additive,
			Rotation = Angle(0,0,GetXDir(), GetYDir())
		};
		
		CreateParticle("ElectroSpark", 0, 0, 0, 0, 10, trailparticles, 5);
	}
	
	if (Type == "Fire")
	{
		var firetrailparticles =
		{
			Prototype = Particles_FireTrail(),
			Size = PV_Linear(10,0)
		};
		CreateParticle("Fire", 0, 0, 0, 0, 10, firetrailparticles);
	}*/
	this->~TravelEffect();
}

public func HitObject(obj)
{
	if(!obj->IsBlocking())
		return false;
		
	Hit();
}

public func Hit()
{
	Explode(SpellDamage);
}

local Plane = 450;

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

