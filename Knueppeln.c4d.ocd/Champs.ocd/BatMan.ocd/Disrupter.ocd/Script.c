
local pR = 100;
local pG = 150;
local pB = 150;
local Speed = 80;
local SpellDamage = 12;
local Size = 45;
local Charge_dur = 15;
local ManaCost = 30;

local counter = 0;
local shooter;

local trailparticles;
local pulseprt;

local x,y;

func Initialize()
{
	SetAction("Travel");
	SetClrModulation(RGBa(0,0,0,0));

	trailparticles =
	{
		Size = PV_Linear(PV_Random(4, 8),0),
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
	clonk->Charge(this, "ChargeStop", Charge_dur, params);
	shooter = clonk;
}

func ChargeInterrupted()
{
	RemoveObject();
}

func ChargeStop(proplist params)
{
	SetAction("Idle");
	var angle = params.new_angle;
	SetVelocity(angle, Speed, 10);
	AddEffect("TheEffect", this, 1, 1, this, GetID());
	AddEffect("HitCheck", this, 1,1, nil,nil, params.cl);

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
	if(time%10)
		CreateParticle("Shockwave2", 0, 0, 0, 0, 10, pulseprt, 1);
		
	DrawParticleLine("Flash", this.x - GetX(), this.y - GetY(), 0, 0, 1, 0, 0, 20, trailparticles);
	this.x = GetX();
	this.y = GetY();
}

func HitEffect()
{
	var props = {
		Size = PV_Linear(20, Size*2),
	    R = pR,
	    G = pG,
	    B = pB,
	    Alpha = PV_Linear(180, 0),
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("Shockwave2", 0, 0, 0, 0, 5, props, 1);

	for(var o in FindObjects(Find_Distance(Size), Find_Func("CanBeHit", this)))
	{
		WeaponDamage(o, SpellDamage);
	}
	
	RemoveObject();
}

func HitObject(obj)
{
	if(obj->~CanBeHit() == false)
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