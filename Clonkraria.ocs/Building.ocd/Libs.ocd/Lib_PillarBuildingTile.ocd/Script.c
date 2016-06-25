/**
	Lib_PillarBuildingTile
	

	@author 
*/

#include Lib_BuildingTile

local Name = "$Name$";
local Description = "$Description$";


func IsPillarBuildingTile() { return true; }

func BuildingCondition()
{
	if (FindObject(Find_AtPoint(), Find_Func("IsPillarBuildingTile"), Find_Exclude(this)))
		return false;

	if (VerticesStuckSemi() == GetVertexNum()+1)
		return false;

	if (FindObject(Find_Exclude(this), Find_Func("IsWallBuildingTile"), Find_Not(Find_Func("IsPreview")),
		Find_AtPoint()))
		//Find_Or(Find_AtRect(-GetObjWidth()/2-1, -GetObjHeight()/2, GetObjWidth()+2, GetObjHeight()), Find_AtRect(-GetObjWidth()/2, -GetObjHeight()/2-1, GetObjWidth(), GetObjHeight()+2))))
		return true;
	
	return false;
}