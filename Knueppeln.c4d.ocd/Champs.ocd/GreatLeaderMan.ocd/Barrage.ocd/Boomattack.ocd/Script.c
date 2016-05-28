/*--
	Boom attack
	Authors: Randrian, Newton, Sven2

	An evil rocket which is hungry on the destruction of Windmills
--*/


/* Init */

local particles_thrust;

public func Construction()
{
	SetAction("Fly");
	SetComDir(COMD_None);
	var fx = AddEffect("Flight", this, 1, 2, this);
	FxFlightTimer(this, fx, 0);
	AddEffect("FlightRotation", this, 1, 1, this);
	
	particles_thrust = new Particles_Thrust(5)
	{
	
	};
	return true;
}

/* Flight */

local rotation = 0;

private func FxFlightRotationTimer(object _this, effect, int time)
{
	rotation += 2;
	if (rotation > 360) rotation = 0;

	this.MeshTransformation = Trans_Rotate(rotation, 0,1);
	return FX_OK;
}

private func FxFlightTimer(object _this, effect fx, int time)
{
	var current_angle = Angle(0, 0, GetXDir(), GetYDir());
	// Attack!
	var x = GetX();
	var y = GetY();
	if (!fx.target || !Random(5))
	{
		for (var obj in FindObjects(Find_Distance(300), Find_ID(Clonk), Find_OCF(OCF_CrewMember), Find_Hostile(GetController()), Find_NoContainer(), Find_Layer(GetObjectLayer())))
		{
			var angle = Angle(x, y, obj->GetX(), obj->GetY());
			var dir = GetTurnDirection(current_angle, angle);
			if (Abs(dir) > 45) continue;
			fx.target = obj;
		}
	}
	
	if(fx.target)
	{
		var angle = Angle(x, y, fx.target->GetX(), fx.target->GetY());
		var dir = GetTurnDirection(current_angle, angle);
		SetVelocity(current_angle + BoundBy(dir, -4, 4), this.speed);
	}
	SetR(Angle(0, 0, GetXDir(), GetYDir()));
	var x = -Sin(GetR(), 5);
	var y = +Cos(GetR(), 5);

	var xdir = GetXDir() / 2;
	var ydir = GetYDir() / 2;
	CreateParticle("FireDense", x, y, PV_Random(xdir - 2, xdir + 2), PV_Random(ydir - 2, ydir + 2), PV_Random(16, 30), particles_thrust, 5);

	return FX_OK;
}

/* Riding */

local riderattach;
local rider;

public func OnMount(clonk)
{
	rider = clonk;
	var iDir = -1;
	if (GetX() > LandscapeWidth()/2) iDir = 1;
	clonk->PlayAnimation("PosRocket", CLONK_ANIM_SLOT_Arms, Anim_Const(0));
	riderattach = AttachMesh(clonk, "main", "pos_tool1", Trans_Mul(Trans_Translate(-1000,2000*iDir,2000)));

	return true;
}

public func OnUnmount(clonk)
{
	clonk->StopAnimation(clonk->GetRootAnimation(10));
	DetachMesh(riderattach);
	return true;
}

/* Contact / Explosion */

public func IsProjectileTarget(object projectile)
{
	return (projectile->GetID() != GetID()) || Hostile(GetController(), projectile->GetController());
}

public func OnProjectileHit(object shot) { return DoFireworks(shot->GetController()); }

public func ContactBottom() { return Hit(); }
public func ContactTop() { return Hit(); }
public func ContactLeft() { return Hit(); }
public func ContactRight() { return Hit(); }

public func Hit() { return DoFireworks(NO_OWNER); }
public func HitObject() { return DoFireworks(NO_OWNER); }

private func DoFireworks(int killed_by)
{
	if (rider) {
		rider->RemoveObject();
	}
	RemoveEffect("Flight",this);
	Explode(30);
	return true;
}

/* Status */

public func IsReflectable() { return true; }

local ActMap = {
	Fly = {
		Prototype = Action,
		Name = "Fly",
		Procedure = DFA_FLOAT,
		Length = 1,
		Delay = 0,
		Wdt = 15,
		Hgt = 27,
	},
};

local FlySpeed = 100;
local Name = "$Name$";
local ContactCalls = true;