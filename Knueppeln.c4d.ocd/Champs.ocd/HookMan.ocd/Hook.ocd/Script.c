#include Projectile

local pR = 150;
local pG = 150;
local pB = 150;
local Speed = 90;
local SpellDamage = 1;
local Size = 10;
local ManaCost = 22;
local LifeTime = 22;

local trailparticles;
local hit;
local hookprt;

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

func FxPullStart(target, fx)
{
	fx.x = -10;
	fx.y = -10;
	fx.angle = Angle(shooter->GetX(), shooter->GetY(), GetX(), GetY(), 10);
}

func FxPullTimer(object target, proplist fx, int time)
{
	if(!shooter)
		return RemoveObject();

	var dist = Distance(fx.x, fx.y, shooter->GetX(), shooter->GetY()); 
	
	if(dist < 5)
	{
		fx.cnt++;
	}
	
	if(ObjectDistance(this, shooter) < Size || (dist < 5 && fx.cnt == 2))
	{
		if(shooter->GetAction() == "Tumble")
			shooter->SetAction("Jump");
		
		if(shooter->GBackSolid(0, -12))
		{
			shooter->SetXDir(0);
			shooter->SetYDir(0);
			shooter->SetAction("Hangle");
		}
		
		if(shooter->GBackSolid(-9, 0) || shooter->GBackSolid(9, 0))
		{
			shooter->SetXDir(0);
			shooter->SetYDir(0);
			shooter->SetAction("Scale");
		}
	
		shooter->SetXDir(shooter->GetXDir(1000)/27, 100);
		shooter->SetYDir(shooter->GetYDir(1000)/27, 100);
				
		return RemoveObject();
	}
	
	if(time > LifeTime*2)
		return RemoveObject();
		
	
	shooter->SetAction("Jump");
	shooter->SetVelocity(fx.angle, Speed + 20, 10);
	
	fx.x = shooter->GetX();
	fx.y = shooter->GetY();
		
}

func TravelEffect(int time)
{
	if(!shooter)
		return RemoveObject();
	
	ClearParticles();
	CreateParticle("Hook", GetXDir()/15, GetYDir()/15, 0, 0, 0, hookprt, 1);
	DrawParticleLine("Shockwave2", 0, 0, shooter->GetX() - GetX() + shooter->GetXDir()/10, shooter->GetY() - GetY() + shooter->GetYDir()/10, 5, 0, 0, 1, trailparticles);
}

public func HitObject(obj)
{
	if(obj == shooter)
		return;
	
	if(hit)
		return;
		
	if (GetEffect("Comeback", this))
		RemoveEffect("Comeback", this);
	
	hit = true;
	Sound("Hits::ProjectileHitLiving?", false, 50);
	var angle = Angle(obj->GetX(), obj->GetY(), shooter->GetX(), shooter->GetY(), 10);
	obj->Fling(Sin(angle, 2, 10), -Cos(angle, 2, 10));
	WeaponDamage(obj, SpellDamage);
	Hit();
}

func HitEffect()
{
	
}

func Trigger()
{
	// || GetEffect("Comeback", this)
	if(GetEffect("Pull", this))
		return;
		
	Hit();
	
	var props =
	{
		//Prototype = Particles_ElectroSpark2(),
		Size = PV_Linear(10,0),
		R = 150,
		G = 150,
		B = 150,
	};
	var r = 10;
	for(var i = 0; i < 360; i+= 5)
		CreateParticle("Flash", 0, 0, Cos(i, r + RandomX(-1,1)), Sin(i, r + RandomX(-1, 1)), 10, props);
}

func Destroy()
{
	RemoveObject();
}

func IsReflectable() { return true; }

func Hit()
{
	if (GetEffect("Comeback", this))
		RemoveEffect("Comeback", this);
	
	SetCategory(C4D_StaticBack);
	
	Sound("hooksnap", false, 50);
	SetXDir(0);
	SetYDir(0);
	AddEffect("Pull", this, 1, 1, this);
}