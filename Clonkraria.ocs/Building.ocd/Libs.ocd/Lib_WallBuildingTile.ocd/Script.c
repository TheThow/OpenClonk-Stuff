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

	if (!GBackSolid() && FindObject(Find_Exclude(this), Find_Func("IsBuildingTile"), Find_Not(Find_Func("IsPreview")),
		Find_Or(Find_OnLine(-GetObjWidth()/2-1, 0, GetObjWidth()/2+1, 0), Find_OnLine(0, -GetObjHeight()/2-1, 0, GetObjHeight()/2+2))))
		//Find_Or(Find_AtRect(-GetObjWidth()/2-1, -GetObjHeight()/2, GetObjWidth()+2, GetObjHeight()), Find_AtRect(-GetObjWidth()/2, -GetObjHeight()/2-1, GetObjWidth(), GetObjHeight()+2))))
		return true;
	
	return false;
}