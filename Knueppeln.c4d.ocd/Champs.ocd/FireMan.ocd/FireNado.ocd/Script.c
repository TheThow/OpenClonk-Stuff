/**
	FireNado
	

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local ManaCost = 30;

local size_x = 25;
local size_y = 30;
local SpellDamage = 30;
local a = 0;
local a2 = 0;

local Speed = 35;
local Durr = 70;
local Charge_Dur = 15;
local firetrailparticles;

func CanBeSucked() { return GetEffect("FireNado", this); }

func Initialize()
{
	SetAction("Travel");
	SetClrModulation(RGBa(0,0,0,0));
	
	firetrailparticles =
	{
		Prototype = Particles_FireTrail(),
		Size = PV_Linear(8,0),
		OnCollision=nil,
	};
}

func Launch(object clonk, int x, int y)
{
	var params = {
		angle = Angle(0,0,x,y)
	};
	SetPosition(GetX(), GetY()-6);
	
	if (clonk.ChampType == ComboMan)
	{
		params.new_angle = params.angle*10;
		ChargeStop(params);
		return;
	}
	
	clonk->Charge(this, "ChargeStop", Charge_Dur, params);
}

func ChargeStop(proplist params)
{
	var angle = params.new_angle;
	SetVelocity(angle, Speed, 10);
	
	AddEffect("FireNado", this, 20, 1 ,this, GetID());
	AddEffect("Remove", this, 20, Durr, this, GetID());
	
	Sound("Fire::Fireball", false, 100);
	Sound("Fire::FuseLoop", false, 50, nil, 1);
	SetLightRange(30, 70);
	SetLightColor(RGB(255, 100, 0));
}

func ChargeInterrupted()
{
	RemoveObject();
}

func ChargeEffect(proplist params)
{
	var cnt = 3;

	for(var i = 0; i < cnt; i++)
	{
		var r = 360/cnt * i;
		CreateParticle("Fire", Sin(r + a2, 20), -Cos(r + a2, 20), PV_Random(-2,2), PV_Random(-2,2), 30, firetrailparticles, 3);
	}
	a2+=360/Charge_Dur/cnt;
}

func FxFireNadoTimer(object target, proplist effect, int time)
{
	for(var o in FindObjects(Find_InRect(-size_x/2, -size_y/2, size_x, size_y), Find_Or(Find_Func("IsReflectable"), Find_Func("CanBeHit", this))))
	{
		if(GetEffect("NadoCD", o))
			continue;
		
		AddEffect("NadoCD", o, 20, 20);
		
		if( o->~IsReflectable())
		{
			var speed = Distance(0, 0, o->GetXDir(), o->GetYDir());
			var angle = RandomX(-20,20);
			
			o->SetVelocity(angle, speed);
			
			continue;
		}
	
		if(o->GetOwner() != GetOwner())
		{
			o->Fling(0, -5);
			o->AddFireHitEffect();
			WeaponDamage(o, SpellDamage);
		}

	}
	
	var detail = size_y*2;
	
	for(var i = 0; i > -detail; i--)
	{
		var r = (360 * 3) * i / detail;
		CreateParticle("Fire", Cos(r + a, size_x * i / detail / 2), size_y/2 + size_y * i / detail, 0, 0, 2, firetrailparticles);
		
		if(time%3)
			a++;
	}
	
	var chaoticspark =
	{
		Size = PV_Linear(1, 0),
		ForceX = PV_KeyFrames(10, 0, PV_Random(-6, 6), 333, PV_Random(-6, -6), 666, PV_Random(6, 6), 1000, PV_Random(-6, 6)),
		ForceY = PV_KeyFrames(10, 0, PV_Random(-8, 5), 333, PV_Random(-8, 5), 666, PV_Random(-10, 10), 1000, PV_Random(-10, 15)),
		Stretch = PV_Speed(1000, 500),
		Rotation = PV_Direction(),
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		R = 255,
		G = PV_Linear(255,100),
		B = PV_Random(0, 100),
		DampingX=950,
		DampingY=950,
		Alpha = PV_Random(100,180),
		BlitMode = GFX_BLIT_Additive
	};
	CreateParticle("Magic", RandomX(-5, 5), RandomX(-10, 10), RandomX(25, -25) + GetXDir(), RandomX(-25, 12) + GetYDir(), 50, chaoticspark, 4);
	return 0;
}


func FxRemoveStop()
{
	Sound("Fire::Spark1", false, 50);
	//CastObjects(Flame, RandomX(2,4), RandomX(10,25));
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
		Speed=1000
	},
};
