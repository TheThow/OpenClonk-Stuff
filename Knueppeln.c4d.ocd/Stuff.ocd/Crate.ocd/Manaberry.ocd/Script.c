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
	clonk->DoMagicEnergy(50, 1);
	
	var props = { 
		DampingX = 950, DampingY = 950,
		Alpha = PV_Linear(255, 0),
		Size = PV_Linear(10, 0),
		R = 50,
		G = 100,
		BlitMode = GFX_BLIT_Additive,
	};
	
	clonk->CreateParticle("Flash", PV_Random(-5, 5), PV_Random(-10,10), PV_Random(-10, 10), PV_Random(-10,10), 20, props, 20);
	RemoveObject();
	return true;
}

public func NutritionalValue() { return 5; }

local Name = "$Name$";
local Description = "$Description$";
local Collectible = 1;
