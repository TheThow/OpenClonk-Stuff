/**
	FireNado
	

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local ManaCost = 45;
local SpellRange = 225;

local Dur = 400;
local Charge_dur = 34;

local angle_prec = 10;

func Initialize()
{
	SetClrModulation(RGBa(0,0,0,0));
}

func Launch(object clonk, int x, int y)
{
	var params = {
		angle = Angle(0,0,x,y, angle_prec),
		clonk = clonk,
		x = x,
		y = y,
	};
	clonk->Charge(this, "ChargeStop", Charge_dur, params);
	
	var props = {
	
		Size = 4,
		R = 100,
		G = 100,
		B = 100,
		Alpha = PV_Linear(255, 0),
		Rotation = PV_Direction()
	};
	var n = 16;
	for (var i = 0; i < n; i++)
	{
		clonk->CreateParticle("BatPrt", Cos(i * 360/n, 20), Sin(i * 360/n, 20), Sin(i * 360/n, 10), -Cos(i * 360/n, 10), Charge_dur, props, 2);
	}
}

func ChargeEffect(proplist params)
{
	if (params.time < 3)
		return;

	if (params.time % 3 == 0)
	{
		var o = CreateObject(ManBat, params.clonk->GetX() - GetX(), params.clonk->GetY() - GetY(), GetOwner());
		o->Set(params.clonk->GetX() + params.x, params.clonk->GetY() + params.y);
		o->Sound("Animals::Bat::Noise*");
	}
}

func ChargeStop(proplist params)
{
	RemoveObject();
}


func ChargeInterrupted(params)
{
	RemoveObject();
}
