/**
	Lib_BuildingElement
	

	@author KKenny
*/

#include Lib_BuildingTile

local Name = "$Name$";
local Description = "$Description$";
local Plane = 200;

func IsWallBuildingTile() { return true; }

func BuildingCondition()
{
	if (FindObject(Find_AtRect(-GetObjWidth()/2, -GetObjHeight()/2, GetObjWidth(), GetObjHeight()), Find_Exclude(this)))
		return false;

	if (!GBackSolid() && FindObject(Find_Exclude(this), Find_Func("IsBuildingTile"), Find_Or(Find_InRect(-9, -2, 18, 4), Find_InRect(-2, -9, 4, 18))))
		return true;
	
	return false;
}