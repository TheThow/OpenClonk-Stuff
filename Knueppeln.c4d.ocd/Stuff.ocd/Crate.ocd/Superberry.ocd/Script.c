/*-- Sproutberry --*/

protected func Hit()
{
	Sound("Hits::SoftHit1");
}

public func Construction()
{
	this.MeshTransformation = Trans_Scale(1500, 1500, 1500);
}

/* Eating */

protected func ControlUse(object clonk, int iX, int iY)
{
	Sound("Clonk::Action::Munch?");
	//clonk->DoMagicEnergy(50, 1);
	
	var props = { 
		DampingX = 950, DampingY = 950,
		Alpha = PV_Linear(255, 0),
		Size = PV_Linear(10, 0),
		R = 255,
		G = 255,
		B = 0,
		BlitMode = GFX_BLIT_Additive,
	};
	
	clonk->CreateParticle("Flash", PV_Random(-5, 5), PV_Random(-10,10), PV_Random(-10, 10), PV_Random(-10,10), 20, props, 20);
	
	AddEffect("Superberry", clonk, 1, 1, nil, GetID());
	
	RemoveObject();
	return true;
}

func FxSuperberryTimer(target, fx, timer)
{
	if (timer > 40*5)
		return -1;
	
	if(target->GetAction() == "Tumble")
		target->SetAction("Jump");
		
	var rgba = SplitRGBaValue(HSL(Random(255), 255, 128));
	
	var props = {
		R = rgba[0],
		G = rgba[1],
		B = rgba[2],
		Size = PV_Linear(5, 0),
		Alpha = PV_Linear(255,0),
		OnCollision = PC_Bounce(),
		ForceY = PV_Gravity(200),
		BlitMode = GFX_BLIT_Additive
	};
	
	target->CreateParticle("StarSpark", PV_Random(-5, 5), PV_Random(-10, 10), PV_Random(-4,4), PV_Random(-20, -5), 20, props, 1);
}

func FxSuperberryDamage()
{
	return 0;
}

public func NutritionalValue() { return 5; }

local Name = "$Name$";
local Description = "$Description$";
local Collectible = 1;
