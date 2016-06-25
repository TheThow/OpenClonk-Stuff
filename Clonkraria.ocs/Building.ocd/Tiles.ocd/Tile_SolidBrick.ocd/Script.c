/*--- Amethyst ---*/

#include Lib_SolidBuildingTile

local Name = "$Name$";
local Description = "$Description$";
local Collectible = 1;

protected func Hit(x, y)
{
	StonyObjectHit(x,y);
	return true;
}

public func Constructed()
{
	inherited(...);
	
	// Correct graphics based on position.
	var available_sprites = 5;
	var tiled_x = GetX() / build_grid_x % available_sprites;
	var tiled_y = GetY() / build_grid_y % available_sprites;
	
	SetGraphics(Format("_%02d_%02d", tiled_x, tiled_y));
}

local Components = {Rock = 1};
public func IsToolProduct() { return true; }