/**
	Lib_BuildingElement
	

	@author KKenny
*/

#include Lib_BuildingTile

local Name = "$Name$";
local Description = "$Description$";
local Plane = 50;

local previous_mat;

func IsWallBuildingTile() { return true; }
local tile_mode = TILE_MODE_LINE;

func Constructed()
{
	/*var x = GetX();
	var y = GetY();
	var w = GetObjWidth()/2;
	var h = GetObjHeight()/2;
	
	previous_mat = GetMaterial();
	
	DrawMaterialQuad("Tunnel-tunnel", 
		x-w, y-h, x+w, y-h, x+w, y+h, x-w, y+h, DMQ_Sub);*/
	return _inherited();
}

func Redraw()
{
	var x = GetX();
	var y = GetY();
	var w = GetObjWidth()/2;
	var h = GetObjHeight()/2;
	
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
	if (FindObject(Find_AtPoint(), Find_Func("IsBuildingTile"), Find_Exclude(this)))
		return false;

	if (VerticesStuckSemi() == GetVertexNum()+1)
		return false;

	if (FindObject(Find_Exclude(this), Find_Func("IsBuildingTile"), Find_Not(Find_Func("IsPreview")),
		Find_Or(Find_OnLine(-GetObjWidth()/2-1, 0, GetObjWidth()/2+1, 0), Find_OnLine(0, -GetObjHeight()/2-1, 0, GetObjHeight()/2+2))))
		//Find_Or(Find_AtRect(-GetObjWidth()/2-1, -GetObjHeight()/2, GetObjWidth()+2, GetObjHeight()), Find_AtRect(-GetObjWidth()/2, -GetObjHeight()/2-1, GetObjWidth(), GetObjHeight()+2))))
		return true;
	
	return false;
}