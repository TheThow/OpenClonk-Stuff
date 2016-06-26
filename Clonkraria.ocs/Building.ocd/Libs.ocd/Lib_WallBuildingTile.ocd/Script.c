/**
	Lib_BuildingElement
	

	@author KKenny
*/

#include Lib_BuildingTile

local Name = "$Name$";
local Description = "$Description$";
local Plane = 5;

local previous_mat;

func IsWallBuildingTile() { return true; }
local tile_mode = TILE_MODE_LINE;

func Constructed()
{
	/*var x = GetX();
	var y = GetY();
	var w = tile_size_x/2;
	var h = tile_size_y/2;
	
	previous_mat = GetMaterial();
	
	DrawMaterialQuad("Tunnel-tunnel", 
		x-w, y-h, x+w, y-h, x+w, y+h, x-w, y+h, DMQ_Sub);*/
	
	//DrawMap(0,0,LandscapeWidth(), LandscapeHeight()/2, "map Tunnel{overlay{mat = Tunnel;};};");
	
	return _inherited();
}

func Redraw()
{
	var x = GetX();
	var y = GetY();
	var w = tile_size_x/2;
	var h = tile_size_y/2;
	
	var name = MaterialName(previous_mat);
	if (previous_mat == -1)
		ClearFreeRect(x-w, y-h, w*2, h*2);
	else
		DrawMaterialQuad(name, 
		x-w, y-h, x+w, y-h, x+w, y+h, x-w, y+h, DMQ_Sub);
}

func Destruct()
{
	//Redraw();
	return _inherited();
}

func Destroy()
{
	//Redraw();
	return _inherited();
}

func BuildingCondition()
{
	if (FindObject(Find_AtPoint(), Find_Func("IsWallBuildingTile"), Find_Exclude(this)))
		return false;

	if (VerticesStuckSemi() == GetVertexNum()+1)
		return false;

	if (FindObject(Find_Exclude(this), Find_Func("IsBuildingTile"), Find_Not(Find_Func("IsPreview")),
		Find_Or(Find_OnLine(-tile_size_x/2-1, 0, tile_size_x/2+1, 0), Find_OnLine(0, -tile_size_y/2-1, 0, tile_size_y/2+2))))
		//Find_Or(Find_AtRect(-tile_size_x/2-1, -tile_size_y/2, tile_size_x+2, tile_size_y), Find_AtRect(-tile_size_x/2, -tile_size_y/2-1, tile_size_x, tile_size_y+2))))
		return true;
	
	return false;
}