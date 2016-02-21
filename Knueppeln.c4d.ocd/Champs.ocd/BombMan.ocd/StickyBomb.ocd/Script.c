
local ManaCost = 15;
local ChargeDuration = 20;
local Target;
local Speed = 30;

local BlastRadius = 30;
local BlastDamage = 15;


func Construction()
{
	SetCategory(C4D_None);
}

func Launch(object clonk, int x, int y)
{
	var angle = Angle(0, 0, x, y, 10);
	var xdir = Sin(angle, Speed, 10);
	var ydir = -Cos(angle, Speed, 10);
	SetSpeed(xdir, ydir);
	Target = clonk;
	SetController(clonk->GetController());
	SoundAt("Hits::BucketHit*", nil, nil, 70, nil, nil, RandomX(-50, 50));
	
	this.wave_particle = 
	{
		Prototype = Particles_Colored(Particles_Flash(), GetPlayerColor(GetController())),
		Size = PV_Linear(0, 20),
		Alpha = 200,
		Attach = ATTACH_Front
	};
	this.spark_particle = 
	{
		Prototype = Particles_Colored(Particles_Glimmer(), GetPlayerColor(GetController())),
		ForceX = PV_Random(-5, 5, 10),
		ForceY = PV_Random(-5, 5, 10),
	};
	AddTimer("Particles", 2);
	AddTimer("StickTo", 2);
}

func Particles()
{
	if (!Random(10)) CreateParticle("Shockwave", 0, 0, 0, 0, 10, this.wave_particle, 1);
	CreateParticle("SphereSpark", PV_Random(-5, 5), PV_Random(-5, 5), PV_Random(-20, 20), PV_Random(-20, 20), PV_Random(20, 30), this.spark_particle, 2);
}

func StickTo()
{
	for (var target in FindObjects(Find_AtPoint(), Find_Func("CanBeHit")))
	{
		if (target->GetOwner() == GetOwner()) continue;
		SetAction("Attach", target);
		Hit();
		return;
	}
}

func IsReflectable()
{
	return true;
}

func Hit()
{
	SetCategory(C4D_StaticBack);
	SetSpeed();
	RemoveTimer("StickTo");
}

func BlowUp()
{
	var direction = GetSurfaceVector();
	if (direction[0] == 0 && direction[1] == 0)
	{
		Explode(this.BlastRadius, false, this.BlastDamage);
		return;
	}
	
	var radius = this.BlastRadius;
	var dmg = this.BlastDamage;
	var x = 0, y = 0;
	
	while (radius > 10)
	{
		var obj = CreateObject(Rock, x, y, GetController());
		obj->ExplosionEffect(radius, 0, 0, 100);
		obj->Explode(radius, true, dmg);
		
		radius /= 2;
		dmg = 2 * dmg / 3;
		x += radius * direction[0];
		y += radius * direction[1];
	}
	RemoveObject();
}

func Blocked(object clonk)
{
	if (GetCategory() != C4D_StaticBack) return;
	if (!Hostile(GetController(), clonk->GetOwner())) return;
	CreateParticle("SphereSpark", PV_Random(-5, 5), PV_Random(-5, 5), PV_Random(-20, 20), PV_Random(-20, 20), PV_Random(20, 100), 30);
	RemoveObject();
}

local ActMap = {

	Attach = {
		Prototype = Action,
		Name = "Attach",
		Procedure = DFA_ATTACH,
	},

};