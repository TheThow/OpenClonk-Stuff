/**
	Lib_PillarBuildingTile
	

	@author 
*/

#include Lib_BuildingTile

local Name = "$Name$";
local Description = "$Description$";


local tile_mode = TILE_MODE_VERTICAL_LINE;


func IsPillarBuildingTile() { return true; }

func Constructed()
{
	AdjustSurroundingMaterial(true, true, false, false);
	return _inherited();
}

func BuildingCondition()
{
	if (FindObject(Find_Not(Find_Func("IsPreview")), Find_AtPoint(), Find_Not(Find_Func("IsWallBuildingTile")), Find_Func("IsBuildingTile"), Find_Exclude(this)))
		return false;

	if (VerticesStuckSemi() == GetVertexNum()+1)
		return false;
	
	if (FindObject(Find_AtPoint(0, GetObjHeight()), Find_Not(Find_Func("IsPreview")), Find_Or(Find_Func("IsSolidBuildingTile"), Find_Func("IsPillarBuildingTile"))) 
		|| GBackSolid(0, (GetObjHeight())))
		return true;
	
	return false;
}