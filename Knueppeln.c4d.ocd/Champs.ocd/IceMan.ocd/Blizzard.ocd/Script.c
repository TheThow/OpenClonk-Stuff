
local ManaCost = 30;
local ChargeDuration = 20;
local Target;
local ShardCount = 10;
local Speed = 90;

local BlastRadius = 10;
local BlastDamage = 10;

local angle;

func Construction()
{
	SetCategory(C4D_StaticBack);
}

func Launch(object clonk, int x, int y)
{
	this.angle = Angle(0,0,x,y);
	var params = {
		angle = this.angle,
		cl = clonk
	};
	clonk->Charge(this, "ChargeStop", ChargeDuration, params);
	Target = clonk;
	
	SetController(clonk->GetController());
}

func ChargeInterrupted()
{
	RemoveObject();
}

func ChargeStop(proplist params)
{
	SoundAt("Liquids::Splash*", 0, 0, false, 100, nil, nil, nil, -90);
	
	var fx = AddEffect("DoTheCharge", Target, 1, 6, nil, GetID());
	fx.obj = this;
}

func FxDoTheChargeStart(object target, effect fx, bool temp)
{
	if (temp) return;
	target->SetAction("Float");
}

func FxDoTheChargeTimer(object target, effect fx, int time)
{
	SoundAt("Liquids::Splash*", 0, 0, false, 100, nil, nil, nil, 600);
	
	if ((ShardCount-- < 0) || (target->GetProcedure() == "Tumble"))
	{
		return FX_Execute_Kill;
	}

	var x = Sin(angle, 15);
	var y = -Cos(angle, 15);
	var shard = target->CreateObject(IceShard, x, y, target->GetOwner());
	shard.angle = angle + RandomX(-10, 10);
	shard.Speed = Speed;
	shard.BlastDamage = BlastDamage;
	shard.BlastRadius = BlastRadius;
	shard->FireNow();
	shard->SetObjDrawTransform(250, 0, 0, 0, 250);
	RemoveEffect("FlightSparks", shard);
	AddEffect("SmallFlightSparks", shard, 1, 1, nil, GetID());
}

func FxDoTheChargeDamage(object target, effect fx, int damage, int cause)
{
	if (damage >= 0)
		return damage;

	RemoveEffect(nil, target, fx);
	
	return damage;
}

func FxDoTheChargeStop(object target, effect fx, int reason, temp)
{
	if (temp) return;
	if (target)
	{
		target->SetAction("Jump");
	}
	if (fx.obj)
	{
		fx.obj->RemoveObject();
	}
}

func FxSmallFlightSparksStart(object target, effect fx, temp)
{
	if (temp) return;
	fx.particles = 
	{
		R = 200,
		G = 200,
		B = 255,
		Size = PV_Linear(2, 0),
		DampingX = 900, DampingY = 900,
		BlitMode = GFX_BLIT_Additive,
		Stretch = PV_Linear(30000, 0)
	};
}

func FxSmallFlightSparksTimer(object target, effect fx, int time)
{
	fx.particles.Rotation = Angle(0, 0, target->GetXDir(), target->GetYDir());
	target->CreateParticle("SphereSpark", 0, 0, 0, 0, 10, fx.particles, 1);
	return 1;
}
