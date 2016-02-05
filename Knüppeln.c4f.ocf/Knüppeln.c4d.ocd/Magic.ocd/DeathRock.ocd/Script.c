/*-- Big Rock --*/


local ManaCost = 50;
local SpellDamage = 35;
local Speed = 50;
local Charge_dur = 40;

local Size = 20;

local LifeTime = 200;
local rotation = 0;

private func Construction()
{
	rotation = Random(360);
	AddTimer("Rotate", 1);
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
	SetCon(20);
}

func ChargeInterrupted()
{
	RemoveObject();
}

func ChargeStop(proplist params)
{
	Unstuck();
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
	AddEffect("Growth", this, 1, 1, this);
}

func FxGrowthTimer()
{
	if (GetCon() >= 100) return -1;
	DoCon(5);
	
	//SetProperty("MeshTransformation", Trans_Scale(200 + con, 200 + con, 200 + con));
}

func FxCheckEnemiesTimer(object target, proplist effect, int time)
{
	for(var o in FindObjects(Find_Distance(20), Find_Or(Find_Func("IsReflectable"), Find_Func("CanBeHit"))))
	{
		if(GetEffect("DeathRockCD", o) || (o->GetOwner() == GetOwner() && time < 30))
		{
			continue;
		}
		
		var angle = Angle(GetX(), GetY(), o->GetX(), o->GetY());
		o->Fling(Sin(angle, 5), -Cos(angle, 5));
		o->AddEarthHitEffect();
		AddEffect("DeathRockCD", o, 1, 30);
		WeaponDamage(o, SpellDamage);
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
	var x = Sin(a, 15, 10);
	var y = -Cos(a, 15, 10);
	
	CreateParticle("Flash", x, y, 0, 0, 5, flashparticle2, 2);
	
	for(var i = a/10 - 90; i <  a/10 + 270; i+= RandomX(3,10))
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
			
			x = x + Sin(i, Size/3 + RandomX(-2, 2));
			y = y - Cos(i, Size/3 + RandomX(-2, 2));
			
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

func Rotate()
{
	rotation += 5;
	SetProperty("MeshTransformation", Trans_Rotate(rotation,0,1,0));
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
	var x, y;

	for(var i = 0; i < GetVertexNum(); i++)
	{
		if(GetContact(i))
		{
			x = GetVertex(i, VTX_X);
			y = GetVertex(i, VTX_Y);
			break;
		}
	}
	
	// No vertex has contact?
	if (x == nil)
	{
		// Just invert the higher velocity..
		if (Abs(xdir) > Abs(ydir))
		{
			SetXDir(-xdir, 100);
			SetYDir(ydir, 100);
		}
		else
		{
			SetXDir(xdir, 100);
			SetYDir(-ydir, 100);
		}
		return;
	}
	
	var angle = Angle(0, 0, xdir, ydir);
	
	var surface = GetSurfaceVector(x, y);
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
	
	var particles = 
	{
		Size = PV_KeyFrames(0, 0, 0, 100, PV_Random(3, 5), 1000, 3),
		R = 180,
		G = 200,
		B = 180,
		Alpha = PV_Linear(255, 0),
		ForceY = PV_Gravity(125),
		CollisionVertex = 0
	};
	SoundAt("Hits::Materials::Rock::Rockfall1");
	CreateParticle("SmokeDirty", PV_Random(-8, 8), PV_Random(-8, 8), PV_Random(-20, 20), PV_Random(-20, 20), PV_Random(10, 60), particles, 400);
	
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
