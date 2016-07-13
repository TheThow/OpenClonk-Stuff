/**
	Super Spectral Lazor
	

	@author 
*/

local ManaCost = 35;

local SpellDamage = 62;
local Speed = 25;
local Durr = 35;

local Size = 15;

local Name = "$Name$";
local Description = "$Description$";

local ChargeDuration = 25;

local MaxLength = 250;
local TargetAngle = 0;
local shooter;


func Initialize()
{
	SetCategory(C4D_StaticBack);
}

func Launch(object clonk, int x, int y)
{
	shooter = clonk;
	SetController(clonk->GetOwner());
	TargetAngle = Angle(0, 0, x, y);
	
	ImaCharginMaLazor(0, 0, Sin(TargetAngle, MaxLength), Cos(TargetAngle, MaxLength) * -1, TargetAngle);
	clonk->Charge(this, "ChargeStop", ChargeDuration, {});
	
	AddEffect("PreLazor", this, 1, 2, this, nil);
	Sound("Flash::charge", nil, nil, nil, 1, nil, 20);
	
	
	
}

func ChargeInterrupted()
{
	Remove();
}

func Remove()
{
	RemoveObject();
}

func ChargeStop()
{
	// Cast Spell
	var from_x = 0;
	var from_y = 0;
	var to_x   = from_x + Sin(TargetAngle, MaxLength);
	var to_y   = from_y - Cos(TargetAngle, MaxLength);
	ImaFirenMaLazor(from_x, from_y, to_x, to_y, TargetAngle);
	Remove();
}

public func HitObject(obj)
{
	if(obj->~CanBeHit() == false)
		return;
	obj->Fling(0, -2, nil, true);
	WeaponDamage(obj, SpellDamage);
}

func FxPreLazorTimer(target, fx, time)
{
	if (!shooter) return -1;
	
	if (time < ChargeDuration - 2)
	{
		var radius = 2 * (ChargeDuration - time);
		for (var a = 0; a < 360; a += RandomX(20, 40))
		{
			var x = Sin(a + RandomX(-5, 5), radius);
			var y = -Cos(a + RandomX(-5, 5), radius);
			
			var prelazor = 
			{
				Prototype = Particles_ElectroSpark2(),
				Size = PV_Linear(RandomX(5,10),5),
				Rotation = TargetAngle,
				R = RandomX(0,200),
				G = RandomX(0,200),
				B = RandomX(0,200),
				Alpha = PV_Linear(100,20),
				OnCollision = PC_Bounce(500),
			};
			
			CreateParticle("StarSpark", x, y, -x/2, -y/2, 10, prelazor, 4);
		}
		return FX_OK;
	}
}

func ImaCharginMaLazor(from_x, from_y, to_x, to_y, angle)
{
	for (var i = 0; i <= MaxLength; i += 5)
	{
		var posx = Sin(angle, i);
		var posy = Cos(angle, i) * -1;
		
		var prelazor = 
		{
			Prototype = Particles_ElectroSpark2(),
			Size = PV_Linear(RandomX(10,20),0),
			Rotation = TargetAngle,
			R = PV_Linear(0, 255),
			G = PV_Linear(0, 255),
			B = PV_Linear(180, 255),
			Alpha = PV_Linear(0,150),
			OnCollision = PC_Bounce(500),
		};
		
		CreateParticle("Flash", posx, posy, 0, 0, ChargeDuration, prelazor);
	}
}

func ImaFirenMaLazor(from_x, from_y, to_x, to_y, angle)
{	
	for (var i = 0; i <= MaxLength; i += 5)
	{
		var posx = Sin(angle, i);
		var posy = Cos(angle, i) * -1;
		
		var lazorflash = 
		{
			Prototype = Particles_ElectroSpark2(),
			Size = PV_Linear(RandomX(20,30),0),
			Rotation = TargetAngle,
			R = RandomX(0,200),
			G = RandomX(0,200),
			B = RandomX(0,200),
			Alpha = PV_Linear(100,20),
			OnCollision = PC_Bounce(500),
		};
		
		CreateParticle("Flash", posx + RandomX(-5,5), posy + RandomX(-5,5), Sin(angle + RandomX(-2, 2), RandomX(2,9)), Cos(angle + RandomX(-2,2), RandomX(2,9)) * -1, 40, lazorflash, 10);
		if (i % 20 == 0) CreateLight(posx, posy, 50, 1, nil, 0, 40);
	}
	SoundAt("Flash::lightstrike", nil, nil, nil, nil, nil, 0);
	
	for (var obj in FindObjects(Find_OnLine(from_x, from_y, to_x, to_y), Find_OCF(OCF_Alive), Find_Exclude(this.shooter), Find_Func("CanBeHit")))
	{
		this->HitObject(obj);
	}
	
	
}








