
local ManaCost = 30;
local ChargeDuration = 20;
local Target;
local ShardCount = 3;
local Radius = 30;
local Speed = 80;

local BlastRadius = 30;
local BlastDamage = 20;

local is_selected = false;
local number = 0;
local offset = 0;
local angle;

func Construction()
{
	SetCategory(C4D_StaticBack);
}

func Launch(object clonk, int x, int y)
{
	var params = {
		angle = Angle(0,0,x,y),
		cl = clonk
	};
	clonk->Charge(this, "ChargeStop", ChargeDuration, params);
	Target = clonk;
	SetController(clonk->GetController());
}

func ChargeStop(proplist params)
{
	Sound("Liquids::Splash*", false, 100, nil, nil, nil, 200);
	
	var a = 360 / ShardCount;
	
	for (var i = 1; i < ShardCount; ++i)
	{
		var other = CreateObject(GetID(), 0, 0, GetController());
		other->Init(a * i, Radius);
		other.number = -i;
		other.Target = this.Target;
	}
	is_selected = true;
	Init(0, Radius);
}

func Init(int offset_, int radius)
{
	AddEffect("IntInFlight", this, 1, 1, this);
	this.offset = offset_;
	this.Radius = radius;
}

func FxIntInFlightStart(target, fx, temp)
{
	if (temp) return;
	fx.particles = 
	{
		R = 255,
		G = 255,
		B = 255,
		Size = PV_Linear(PV_Random(4, 8), 0),
		OnCollision = PC_Stop(),
		CollisionVertex = 0,
		ForceX = PV_Linear(0, PV_Random(-10, 10, 10)),
		ForceY = PV_Linear(0, PV_Random(-10, 10, 10)),
		DampingX = 900, DampingY = 900
	};
}

func FxIntInFlightTimer(target, fx, int time)
{
	if (!Target) return RemoveObject();

	angle = 5 * time + offset;
	var radius = Min(time, this.Radius);
	if (is_selected)
	{
		radius += 5;
		fx.particles.BlitMode = GFX_BLIT_Additive;
		fx.particles.R = 100;
		fx.particles.G = 100;
	}
	else
	{
		fx.particles.BlitMode = nil;
		fx.particles.R = 255;
		fx.particles.G = 255;
	}
	
	CreateParticle("StarFlash", PV_Random(-2, 2), PV_Random(-2, 2), 0, 0, 15, fx.particles, 3);
	var x = Sin(angle, radius);
	var y = -Cos(angle, radius);
	SetPosition(Target->GetX() + x, Target->GetY() + y);
}

func FireNow()
{
	SetCategory(C4D_Object);
	SetVelocity(angle, Speed);
	RemoveEffect("IntInFlight", this);
	AddEffect("HitCheck", this, 1,1, nil,nil, Target);
}

public func HitObject(obj)
{
	if(obj->~CanBeHit() == false)
		return;
		
	Hit();
}

func Hit()
{
	this->Call(IceProjectile.Iceplosion, BlastRadius, BlastDamage);
}