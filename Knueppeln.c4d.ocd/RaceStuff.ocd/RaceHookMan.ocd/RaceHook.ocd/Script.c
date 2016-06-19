#include Projectile

local pR = 150;
local pG = 150;
local pB = 150;
local Speed = 100;
local SpellDamage = 1;
local Size = 10;
local ManaCost = 1;
local LifeTime = 15;

local trailparticles;
local hit;
local hookprt;

func Blocked(obj)
{
	DestroyEffect();
	Destroy();
}

func CanBeSucked() { return false; }

func IsReflectable() { return false; }

func InitEffect()
{
	Sound("Objects::Weapons::WeaponSwing1", false, 100);
	SetLightRange(30, 70);
	SetLightColor(RGB(150, 150, 150));

	trailparticles =
	{
		Size = 5,
		R = pR,
		G = pG,
		B = pB,
		Attach=ATTACH_Front
	};
	
	var angle = Angle(0, 0, GetXDir(), GetYDir());
	
	hookprt = 
	{
		Size = 8,
		R = pR,
		G = pG,
		B = pB,
		Attach=ATTACH_Front|ATTACH_MoveRelative,
		Rotation = angle
	};
	
	
	AddEffect("Life", this, 1, LifeTime, this);
	hit = false;
}

func FxLifeStop()
{
	if(!GetEffect("Pull", this))
	{
		AddEffect("Comeback", this, 1, 1, this);
	}
}

func FxComebackTimer(object target, proplist effect, int time)
{
	if(!this)
		return;
	
	if(!shooter)
		return RemoveObject();

	if(ObjectDistance(this, shooter) < Size)
		return RemoveObject();

	var angle = Angle(GetX(), GetY(), shooter->GetX(), shooter->GetY(), 10);
	
	SetVelocity(angle, Speed + 40, 10);
}

func FxPullStart(target, fx, temp)
{
	if(temp)
		return;
		
	fx.x = GetX();
	fx.y = GetY();
	fx.ox = shooter->GetX();
	fx.oy = shooter->GetY();
	fx.angle = Angle(shooter->GetX(), shooter->GetY(), GetX(), GetY(), 10);
}

func FxPullTimer(object target, proplist fx, int time)
{
	if(!shooter)
		return RemoveObject();

	shooter->SetAction("Jump");
	shooter->SetVelocity(fx.angle, Speed + 20, 10);

	var dist = Distance(fx.x, fx.y, shooter->GetX(), shooter->GetY()); 
	
	if(dist < 5)
	{
		fx.cnt++;
	}
	
	var cd1 = Distance(GetX(), GetY(), fx.ox, fx.oy); 
	var cd2 = Distance(shooter->GetX(), shooter->GetY(), fx.ox, fx.oy); 
	
	if(cd1-cd2 < Size || (dist < 5 && fx.cnt == 2))
	{
		shooter->SetXDir(shooter->GetXDir(1000)/15, 100);
		shooter->SetYDir(shooter->GetYDir(1000)/15, 100);
	
		return RemoveObject();
	}
	
	if(time > LifeTime*2)
		return RemoveObject();
		
	//fx.angle = Angle(shooter->GetX(), shooter->GetY(), GetX(), GetY(), 10);

	
	fx.x = shooter->GetX();
	fx.y = shooter->GetY();
		
}

func TravelEffect(int time)
{
	if(!shooter)
		return RemoveObject();
	var obj = FindObject(Find_ID(SawBlade), Find_AtPoint());
	if(obj)
	{
		obj->HitByHook(this);
		DestroyEffect();
		Destroy();
		return;
	}
	
	ClearParticles();
	CreateParticle("Hook", GetXDir()/15, GetYDir()/15, 0, 0, 0, hookprt, 1);
	DrawParticleLine("Shockwave2", GetXDir()/15, GetYDir()/15, shooter->GetX() - GetX() + shooter->GetXDir()/10, shooter->GetY() - GetY() + shooter->GetYDir()/10, 5, 0, 0, 1, trailparticles);
}

public func HitObject(obj)
{

	if(obj->GetID() == RaceSawBlade)
	{
		DestroyEffect();
		Destroy();
	}

	if(obj->GetID() != Amethyst)
		return;
	
	if(hit)
		return;
		
	
		
		
	if (GetEffect("Comeback", this))
		RemoveEffect("Comeback", this);
	
	hit = true;

	if (GetEffect("Comeback", this))
			RemoveEffect("Comeback", this);
	
	SetCategory(C4D_StaticBack);
	
	Sound("hooksnap", false, 50);
	SetXDir(0);
	SetYDir(0);
	AddEffect("Pull", this, 1, 1, this);

}

func HitEffect()
{
	
}

func Trigger()
{
	shooter->SetXDir(shooter->GetXDir(1000)/15, 100);
	shooter->SetYDir(shooter->GetYDir(1000)/15, 100);

	DestroyEffect();
	Destroy();
}

func Destroy()
{
	RemoveObject();
}

func DestroyEffect()
{
	Sound("Hits::Materials::Metal::LightMetalHit1", false, 50);

	var destroyprt =
	{
		Size = 5,
		Alpha = PV_Linear(255, 0),
		R = pR,
		G = pG,
		B = pB,
		ForceY = GetGravity(),
	};
	
	DrawParticleLine("Shockwave2", 0, 0, shooter->GetX() - GetX() + shooter->GetXDir()/10, shooter->GetY() - GetY() + shooter->GetYDir()/10, 5, PV_Random(-5,5), PV_Random(-15, 5), 20, destroyprt);

}


func Hit()
{
	DestroyEffect();
	Destroy();
}