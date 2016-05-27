#include Projectile

local pR = 150;
local pG = 150;
local pB = 150;
local Speed = 75;
local SpellDamage = 15;
local Size = 10;
local ManaCost = 25;
local LifeTime = 25;

local trailparticles;
local hit;

func InitEffect()
{
	Sound("electro_shot", false, 100);
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
	AddEffect("Life", this, 1, LifeTime, this);
	hit = false;
}

func FxLifeStop()
{
	if(!GetEffect("Pull", this))
	{
		while(RemoveVertex());
		AddEffect("Comeback", this, 1, 1, this);
	}
}

func FxComebackTimer(object target, proplist effect, int time)
{
	if(ObjectDistance(this, shooter) < Size)
		RemoveObject();

	var angle = Angle(GetX(), GetY(), shooter->GetX(), shooter->GetY(), 10);
	
	SetVelocity(angle, Speed + 20, 10);
}

func FxPullTimer(object target, proplist effect, int time)
{
	var angle = Angle(shooter->GetX(), shooter->GetY(), GetX(), GetY(), 10);
	shooter->SetAction("Jump");
	shooter->SetVelocity(angle, Speed + 20, 10);
	
	if(ObjectDistance(this, shooter) < Size)
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
		
		RemoveObject();
	}
	
	if(this && time > LifeTime*2)
		RemoveObject();
}

func TravelEffect(int time)
{
	DrawParticleLine("Shockwave2", 0, 0, shooter->GetX() - GetX() + shooter->GetXDir()/10, shooter->GetY() - GetY() + shooter->GetYDir()/10, 5, 0, 0, 1, trailparticles);
}

public func HitObject(obj)
{
	if(obj == shooter)
		return;
	
	if(hit)
		return;
	
	hit = true;

	WeaponDamage(obj, SpellDamage);
	var angle = Angle(obj->GetX(), obj->GetY(), shooter->GetX(), shooter->GetY(), 10);
	obj->Fling(Sin(angle, 6, 10), -Cos(angle, 6, 10));
	Hit();
}

func HitEffect()
{
	
}

func Trigger()
{
	if(!GetEffect("Pull", this) && !GetEffect("Comeback", this))
		Hit();
}

func Destroy()
{
	RemoveObject();
}

func IsReflectable() { return true; }

func Hit()
{
	SetXDir(0);
	SetYDir(0);
	AddEffect("Pull", this, 1, 1, this);
}