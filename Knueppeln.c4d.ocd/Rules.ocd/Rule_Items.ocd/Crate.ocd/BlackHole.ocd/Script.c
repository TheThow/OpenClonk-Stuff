/*-- Ore --*/

local probability = 15;

func IsKnueppelItem() { return true; }

protected func Construction()
{
	var graphic = Random(5);
	if(graphic)
		SetGraphics(Format("%d",graphic));
}

protected func Hit(x, y)
{
	StartBlackHole();
	return true;
}

func StartBlackHole()
{
	Collectible = 0;
	SetCategory(C4D_StaticBack);
	
	AddEffect("Grow", this, 1, 1, this);
}

public func IsFoundryIngredient() { return true; }

local Collectible = 1;
local Name = "$Name$";
local Description = "$Description$";
local Plane = 460;