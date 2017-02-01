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

local probability = 25;

func IsKnueppelItem() { return true; }
func CanBeSucked() { return true; }
func IsReflectable() { return true; }

local SquidFx = new Effect {
	Damage = func() {
		return 0;
	},
	Timer = func() {
		this.timer++;
		Target->SetPosition(Target->GetX(), Target->GetY()-1);
		
		if(Target.swimming == false)
			Target->StartSwim();
	
		if(this.timer >= 40)
		{
			var particles =
			{
				ForceY = PV_Random(-2, 2),
				ForceX = PV_Random(-2, 2),
				DampingX = 900, DampingY = 900,
				Alpha = PV_Linear(255, 0),
				R = 50, G = 50, B = 100,
				Size = PV_Linear(PV_Random(10, 20), PV_Random(40, 60)),
				Phase = PV_Random(0, 15),
				Attach = ATTACH_Front
			};
			
			var dummy = Target->CreateObject(Dummy);
			dummy.Plane = 940;
			dummy.Visibility = VIS_All;
			
			for (var i = 0; i < 360; i+=2)
			{
				dummy->CreateParticle("SmokeThick", 0, 0, Cos(i, RandomX(10,30)), Sin(i, RandomX(10,30)), PV_Random(360, 400), particles, 2);
				dummy->CreateParticle("SmokeThick", 0, 0, Cos(i, RandomX(30,60)), Sin(i, RandomX(30,60)), PV_Random(360, 400), particles, 2);
				dummy->CreateParticle("SmokeThick", 0, 0, Cos(i, RandomX(60,100)), Sin(i, RandomX(60,100)), PV_Random(360, 400), particles, 2);
			}
			
			dummy->CreateEffect(MonsterBall.RemoveFx, 1, 400);
			
			Target->RemoveObject();
			return -1;
		}
	}
};

local RemoveFx = new Effect
{
	Timer = func()
	{
		Target->RemoveObject();
		return -1;
	}
	
};

local WormySpamFx = new Effect
{
	Timer = func()
	{
		if (Time == 10) WormyMcWormFace->Appear(10, Target->GetX(), Target->GetY(), Target->GetController());
		if (Time == 20) WormyMcWormFace->Appear(10, Target->GetX(), Target->GetY(), Target->GetController());
		if (Time == 30) WormyMcWormFace->Appear(10, Target->GetX(), Target->GetY(), Target->GetController());
		if (Time == 40) WormyMcWormFace->Appear(10, Target->GetX(), Target->GetY(), Target->GetController());
		if (Time == 50) WormyMcWormFace->Appear(10, Target->GetX(), Target->GetY(), Target->GetController());
		if (Time == 55) return -1;
		
	}
};

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
	
	monsters = [Piranha, Squid, WormyMcWormFace, Howler];
	
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
	Sound("Hits::Materials::Glass::GlassHit*", false, 50);
	
	if(!thrown)
		return;
	
	Sound("Items::monsterball", false, 100);
	
	var monster = monsters[Random(GetLength(monsters))];
	
	if (monster == Piranha)
	{
		CreateObject(monster, 0, - 5, -1);
	}
	
	if (monster == Howler)
	{
		CreateObject(monster, 0, - 5, -1);
	}
	
	if (monster == Bat)
	{
		for (var i = 0; i < 7; i++)
		{
			CreateObject(monster, RandomX(-5,5), -5, -1);
		}
	}
	
	if (monster == Squid)
	{
		var squid = CreateObject(monster, RandomX(-5,5), -15, -1);
		squid->SetCategory(C4D_StaticBack);
		squid->CreateEffect(SquidFx, 1, 1);
	}
	
	if (monster == WormyMcWormFace)
	{
		WormyMcWormFace->Appear(10, GetX(), GetY(), GetController());
		if (Random(10) < 3) WormyMcWormFace->Appear(10, GetX(), GetY(), GetController());
		if (Random(10) < 2) WormyMcWormFace->Appear(10, GetX(), GetY(), GetController());
		if (Random(100) < 2) this->CreateObject(Dummy)->CreateEffect(WormySpamFx, 1, 1);
	}
	
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