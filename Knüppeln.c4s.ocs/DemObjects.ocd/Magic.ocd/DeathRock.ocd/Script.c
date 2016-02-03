/*-- Big Rock --*/


local ManaCost = 50;
local SpellDamage = 35;
local Speed = 60;
local Charge_dur = 40;

local Size = 20;

local LifeTime = 200;

private func Initialize()
{
 	SetProperty("MeshTransformation", Trans_Rotate(RandomX(0,359),0,1,0));
  	//SetProperty("MeshTransformation", Trans_Mul(Trans_Scale(1000,1000,1000),Trans_Rotate(RandomX(0,359),0,1,0), Trans_Rotate(RandomX(0,359), 1, 0, 0)));
  	SetClrModulation(RGBa(255,255,255,0));
  	SetAction("Travel");
}

func Launch(object clonk, int x, int y)
{
	var params = {
		angle = Angle(0,0,x,y),
		cl = clonk
	};
	clonk->Charge(this, "ChargeStop", Charge_dur, params);
}

func ChargeStop(proplist params)
{
	var angle = params.new_angle;
	SetVelocity(angle, Speed, 10);
	
	Sound("Fire::BlastLiquid3", false, 100);
	
	AddEffect("CheckEnemies", this, 20,1, this);
	
	SetLightRange(30, 70);
	SetLightColor(RGB(50, 255, 50));
	
	AddEffect("Particles", this, 20, 1, this);
	AddEffect("Life", this, 20, LifeTime, this);
	SetClrModulation(RGBa(255,255,255,255));
	SetAction("Idle");
	
	var dir = 1;
	if(GetXDir())
		dir = GetXDir() / Abs(GetXDir());
	SetRDir(10*dir);
}

func FxCheckEnemiesTimer()
{
	for(var o in FindObjects(Find_Distance(20), Find_Or(Find_Func("IsReflectable"), Find_Func("CanBeHit"))))
	{
		if(GetEffect("DeathRockCD", o) || (o->GetOwner() == GetOwner()))
		{
			continue;
		}
		
		Log("Hit");
		var angle = Angle(GetX(), GetY(), o->GetX(), o->GetY());
		o->Fling(Sin(angle, 5), -Cos(angle, 5));
		o->AddEarthHitEffect();
		AddEffect("DeathRockCD", o, 1, 30);
		o->DoEnergy(-SpellDamage);
		Sound("Hits::GeneralHit1", false, 50);
	}
}

func ChargeEffect(proplist params)
{
	var flashparticle2 =
	{
		Alpha = 30,
		Size = Size/2,
		R = 50,
		G = 255,
		B = 50,
		BlitMode = GFX_BLIT_Additive,
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	
	var a = params.new_angle;
	var x = Sin(a, 10, 10);
	var y = -Cos(a, 10, 10);
	
	CreateParticle("Flash", x, y, 0, 0, 5, flashparticle2, 2);
	
	for(var i = 0; i < 360; i+= RandomX(3,10))
	{
		if(!Random(5))
		{
			var props =
			{
				Size = PV_Linear(5,4),
				R = 50, G = 255, B = 50,
				Alpha = PV_KeyFrames(0, 0, 200, 100, 0, 200, 255, 1000, 0),
				Phase = PV_Random(0, 5),
				BlitMode = GFX_BLIT_Additive,
				Rotation = i + 180,
			};
			
			x = x + Sin(i, Size/3);
			y = y - Cos(i, Size/3);
			
			var xdir = Sin(i + 180, 15);
			var ydir = -Cos(i + 180, 15);
			
			CreateParticle("Lightning", x, y, xdir, ydir, 10, props, 1);
		}
	}
}

func FxParticlesTimer(object target, proplist effect, int time)
{
	var sharpflame =
	{
		Size = 10,
		R = 50,
		G = 255,
		B = 50,
		Alpha = PV_Linear(60, 0),
		Rotation = PV_Random(0, 360),
		Phase = PV_Random(0, 5),
		OnCollision = 0,
		BlitMode = GFX_BLIT_Additive,
	};
	
	CreateParticle("FireSharp", PV_Random(-15, 15), PV_Random(-15, 15), RandomX(-1, 1), PV_Random(-1,-2), 20, sharpflame, 4);
	
	var lightning =
	{
		Prototype = Particles_ElectroSpark2(),
		Size = PV_Linear(PV_Random(3,7),0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Random(0,360),
		R = 50,
		G = 255,
		B = 50,
	};
	
	CreateParticle("Lightning", RandomX(-12, 12), RandomX(-12, 12), 0, 0, 10, lightning, 2);
}

func Hit(xdir, ydir)
{
	Bounce(xdir, ydir);
	
	var dir = 1;
	if(xdir)
		dir = xdir / Abs(xdir);
	
	SetRDir(10 * dir);
	Sound("Hits::Materials::Rock::RockHit?");
}

func Bounce(int xdir, int ydir)
{

	var angle = Angle(0, 0, xdir, ydir);
	
	var surface = GetSurfaceVector(0, 0);
	var surface_angle = Angle(0, 0, surface[0], surface[1]);
	var angle_diff = GetTurnDirection(angle - 180, surface_angle);
	var new_angle = surface_angle + angle_diff;
	
	var speed = Distance(0, 0, xdir, ydir);
	SetXDir(Sin(new_angle, speed), 100);
	SetYDir(-Cos(new_angle, speed), 100);
}

func FxLifeStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;
	
	RemoveObject();
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
