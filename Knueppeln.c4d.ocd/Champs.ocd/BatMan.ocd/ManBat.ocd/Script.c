/**
	Bat
	Flutters around at night and in caves. The bat heals itself by
	biting prey animals or clonks. The less energy it has, the more
	agressive it will be towards preys.

	@author Win, Maikel
*/

local tx;
local ty;
local range = 40;
local maxspeed = 30;

local TravelFx = new Effect {

	Timer = func() {
		
		this.timer++;
		var x = Target.tx;
		var y = Target.ty;
		
		for (var o in Target->FindObjects(Find_Distance(15), Find_Not(Find_Owner(Target->GetOwner())), Find_Func("CanBeHit", Target)))
		{
			if (GetEffect("BatBiteCD", o))
				continue;
			
			AddEffect("BatBiteCD", o, 1, 20);
			Target->BitePrey(o);
		}
		
		if (x == nil || y == nil)
			return;
		
		if(this.cx == nil || this.cy == nil || Distance(Target->GetX(), Target->GetY(), this.cx, this.cy) < 10)
		{
			this.cx = x + Cos(Random(360), Random(Target.range));
			this.cy = y + Sin(Random(360), Random(Target.range));
		}
		
		var angle = Angle(Target->GetX(), Target->GetY(), this.cx, this.cy);
		Target->AddVelocity(angle, 3);
		
		var s = Distance(0, 0, Target->GetXDir(), Target->GetYDir());
		
		if (s > Target.maxspeed)
			Target->SetVelocity(Angle(0, 0, Target->GetXDir(), Target->GetYDir()), Target.maxspeed);
		
		Target->UpdateFlightAppearance();
		
		if (this.timer > 400)
		{
			Target->Kill();
			return -1;
		}
	}
	
};

func CanBeHit() { return true; }
func CanBeSucked() { return true; }

func Set(x, y)
{
	tx = x;
	ty = y;
}

private func Construction()
{
	Fly();
	CreateEffect(TravelFx, 1, 1);
}

public func IsManBat() { return true; }

/*-- Behavior --*/

// When some object hurts the bat.
public func CatchBlow(int damage, object obj)
{
	// Only catch a blow if alive.
	if (GetAction() == "Dead")
		return;
	// Make a sound.
	Sound("Animals::Bat::Noise*");
	return;
}

private func UpdateFlightAppearance()
{
	// The rotation around the z-axis depends on the angle of the bat.
	var angle = Angle(0, 0, GetXDir(), GetYDir());
	var dir = 1;
	if (GetXDir() > 0)
	{
		angle = BoundBy(angle, 30, 150);
		dir = 1;
	}
	else
	{
		angle = 2 * 270 - BoundBy(angle, 210, 330);
		dir = -1;	
	}
	this.MeshTransformation = Trans_Mul(Trans_Scale(1000, dir * 1000, 1000), Trans_Rotate(angle, 0, 0, 1));
	return;
}


// Lets the bat start flying.
private func Fly()
{
	// Stop current animation and start flying.
	PlayAnimation("Fly", 1, Anim_Linear(0, 0, GetAnimationLength("Fly"), 10, ANIM_Loop));
	SetAction("Flight");
	SetComDir(COMD_None);	
	return;
}

// Lets the bat bite a pray animal, assumes the bat is close to the prey.
private func BitePrey(object prey)
{
	// A small blow to the prey and heal the bat itself.
	WeaponDamage(prey, 10);
	// Make a bite/attack sound.
	Sound("Hits::ProjectileHitLiving*");
	return;
}



/*-- Dead State --*/

private func Death()
{
	var props = {
		Size = PV_Linear(PV_Random(2, 4), 0),
		CollisionVertex = 500,
		OnCollision = PC_Bounce(200),
		ForceY = PV_Gravity(300),
		R = 128,
		G = 0,
		B = 0
	};
	
	CreateParticle("Flash", 0, 0, PV_Random(-60, 60), PV_Random(-60, 30), 140, props, 45);
	
	RemoveObject();
}



/*-- Properties --*/

private func Definition(proplist def)
{
	def.PictureTransformation = Trans_Mul(Trans_Rotate(-65, 0, 1, 0), Trans_Rotate(-35, 0, 0, 1));
	return _inherited(def, ...);
}

local Name = "$Name$";
local Description = "$Description$";

local MaxEnergy = 10000;
local MaxBreath = 180;
local NoBurnDecay = 1;
local ContactIncinerate = 10;
local BorderBound = C4D_Border_Sides | C4D_Border_Top | C4D_Border_Bottom;
local ContactCalls = true;

local ActMap = {
	Hang = {
		Prototype = Action,
		Name = "Hang",
		Procedure = DFA_FLOAT,
		Speed = 100,
		Accel = 16,
		Decel = 16,
		Length = 1,
		Delay = 0,
		FacetBase = 1,
		NextAction = "Hang",
	},
	Flight = {
		Prototype = Action,
		Name = "Flight",
		Procedure = DFA_FLOAT,
		Speed = 100,
		Accel = 16,
		Decel = 16,
		Length = 1,
		Delay = 0,
		FacetBase = 1,
		NextAction = "Flight",
	},
	Dead = {
		Prototype = Action,
		Name = "Dead",
		Procedure = DFA_NONE,
		Length = 1,
		Delay = 0,
		FacetBase=1,
		Directions = 2,
		FlipDir = 1,
		NextAction = "Hold",
		NoOtherAction = 1,
		ObjectDisabled = 1,
	}
};