/*--- Amethyst ---*/

#include Lib_WallBuildingTile

local Name = "$Name$";
local Description = "$Description$";
local Collectible = 1;

protected func Hit(x, y)
{
	StonyObjectHit(x,y);
	return true;
}

local Components = {Wood = 1};
public func IsToolProduct() { return true; }