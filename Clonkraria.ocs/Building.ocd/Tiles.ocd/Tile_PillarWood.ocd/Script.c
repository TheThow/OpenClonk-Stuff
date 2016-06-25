/**
	Tile_PillarWood
	

	@author 
*/

#include Lib_PillarBuildingTile

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