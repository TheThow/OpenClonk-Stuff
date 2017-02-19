
local pR = 100;
local pG = 150;
local pB = 150;
local Speed = 80;
local SpellDamage = 10;
local Size = 40;
local Charge_dur = 15;
local ManaCost = 30;
local Dur = 160;

local counter = 0;
local shooter;

local trailparticles;
local pulseprt;

local x,y;

local dummy;

func IsReflectable() { return true; }
func CanBeSucked() { return true; }
public func IsDangerous4AI() { return true; }

local DisruptorHitFx = new Effect {

	Timer = func() {
	
		this.timer++;
		
		Target->CreateParticle("Lightning", RandomX(-5, 5), RandomX(-10, 10), 0, 0, 10, this.lightning, 2);
		
		if(this.timer > this.dur)
			return -1;
	}

};

func Initialize()
{
	SetAction("Travel");
	SetClrModulation(RGBa(0,0,0,0));

	trailparticles =
	{
		Size = PV_Linear(PV_Random(3, 6),0),
		BlitMode = GFX_BLIT_Additive,
		Alpha = PV_Linear(255, 0),
		R = pR,
		G = pG,
		B = pB,
	};
	
	pulseprt =
	{
		Size = PV_Linear(1, 20),
		BlitMode = GFX_BLIT_Additive,
		Alpha = PV_Linear(255, 0),
		R = 0,
		G = 255,
		B = 255,
		Attach = ATTACH_Front|ATTACH_MoveRelative,
	};
}

func Launch(object clonk, int x, int y)
{
	var params = {
		angle = Angle(0,0,x,y),
		cl = clonk
	};
	
	shooter = clonk;
	dummy = CreateObject(Dummy, x, y, GetOwner());
	dummy.Visibility = VIS_All;
	
	var chargeprt =
	{
		Size = 4,
		BlitMode = GFX_BLIT_Additive,
		R = pR,
		G = pG,
		B = pB,
		Rotation = PV_Step(20, 0),
		Attach=ATTACH_Front|ATTACH_MoveRelative,
	};
	
	if (clonk.ChampType == ComboMan)
	{
		params.new_angle = params.angle*10;
		ChargeStop(params);
		return;
	}
	
	dummy->CreateParticle("BatPrt", 0, 0, 0, 0, 0, chargeprt, 2);

	clonk->Charge(this, "ChargeStop", Charge_dur, params);
}

func ChargeEffect(proplist params)
{
	var a = params.new_angle;
	var dx = Sin(a, 10, 10);
	var dy = -Cos(a, 10, 10);
	
	dummy->SetPosition(GetX() + dx, GetY() + dy);
}

func ChargeInterrupted()
{
	dummy->RemoveObject();
	RemoveObject();
}

func ChargeStop(proplist params)
{
	dummy->RemoveObject();
	SetAction("Idle");
	var angle = params.new_angle;
	SetVelocity(angle, Speed, 10);
	AddEffect("TheEffect", this, 1, 1, this, GetID());
	AddEffect("HitCheck", this, 1,1, nil,nil, params.cl);
	Sound("BatMan::disruptor_shot", false, 30);

	SetLightRange(30, 70);
	SetLightColor(RGB(255, 255, 255));
}

func FxTheEffectStart(object target, proplist effect, int temporary)
{
	if(temporary)
		return;
		
	this->~InitEffect();
}

func FxTheEffectTimer(object target, proplist effect, int time)
{
	this->~TravelEffect(time);
}

func InitEffect()
{
	SetLightRange(30, 70);
	SetLightColor(RGB(200, 215, 255));
	
	this.x = GetX();
	this.y = GetY();
}

func TravelEffect(int time)
{
	if(time%4==0)
		CreateParticle("Shockwave2", 0, 0, 0, 0, 10, pulseprt, 3);
		
	DrawParticleLine("Flash", this.x - GetX(), this.y - GetY(), 0, 0, 1, 0, 0, 20, trailparticles);
	this.x = GetX();
	this.y = GetY();
}

func HitEffect()
{
	var props = {
		Size = PV_Linear(20, Size*3),
	    R = pR,
	    G = pG,
	    B = pB,
	    Alpha = PV_Linear(180, 0),
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("Shockwave2", 0, 0, 0, 0, 10, props, 1);
	
	var props2 = {
		Size = PV_Linear(Size*2, 0),
	    R = pR,
	    G = pG,
	    B = pB,
	    Alpha = PV_Linear(255, 0),
		BlitMode = GFX_BLIT_Additive,
		Rotation=PV_Step(10,0, 1),
	};

	var props3 = {
		Prototype = props2,
		Rotation=PV_Step(10,0, 1),
	};
	
	CreateParticle("StarSpark", 0, 0, 0, 0, 10, props2, 2);
	CreateParticle("StarSpark", 0, 0, 0, 0, 10, props3, 2);
	
	var particles =
	{
		ForceY = PV_Random(-2, 2),
		ForceX = PV_Random(-2, 2),
		DampingX = 900, DampingY = 900,
		Alpha = PV_Linear(PV_Random(170, 140), 0),
		R = 200, G = 250, B = 250,
		Size = 40,
		Phase = PV_Random(0, 15),
		BlitMode = GFX_BLIT_Additive,
	};
	
	CreateParticle("SmokeThick", PV_Cos(PV_Random(0, 360), PV_Random(0, Size-10)), PV_Sin(PV_Random(0, 360), PV_Random(0, Size-10)), 0, 0, 10, particles, 25);

	var lightning =
	{
		Prototype = Particles_ElectroSpark2(),
		Size = PV_Linear(PV_Random(5,8),0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Random(0,360),
		R = 0,
		G = 250,
		B = 250,
		Attach = ATTACH_Front | ATTACH_MoveRelative,
	};

	var d = Dur;
	if (shooter && shooter.ChampType == ComboMan)
		d = Dur/2;

	for (var o in FindObjects(Find_Distance(Size), Find_Func("CanBeHit", this)))
	{
		if (!o)
			continue;
		if (o->GetID() == Clonk)
		{
			o->DisableCasting(d);
			var fx = o->CreateEffect(DisruptorHitFx, 1, 1);
			fx.dur = d;
				
			fx.lightning = lightning;
		}
			
		WeaponDamage(o, SpellDamage);
	}

	SoundAt("BatMan::disruptor_hit1",0, 0, { volume = 100 });
	SoundAt("BatMan::disruptor_hit2", 0, 0, { volume = 20 });
	
	RemoveObject();
}


func HitObject(obj)
{
	if(obj->~CanBeHit(this) == false)
		return;
		
	Hit();
}

func Hit(int xdir, int ydir)
{
	HitEffect();
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