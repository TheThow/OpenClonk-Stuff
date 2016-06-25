/**
	Lib_BuildingElement
	

	@author KKenny
*/

#include Lib_BuildingElement
#include Library_Stackable

func MaxStackCount() { return 48; }
func InitialStackCount() { return 4; }

local Name = "$Name$";
local Description = "$Description$";

local build_offset_x = 0;
local build_offset_y = 0;

local build_grid_x = 10;
local build_grid_y = 10;

func IsBuildingTile() { return true; }
func CanBeHitByPickaxe() { if (GetCategory() == C4D_StaticBack) return true; return false; }

func Construction()
{
	SetClrModulation(RGBa(180, 180, 180, 255));
	return _inherited();
}

func Constructed()
{
	SetCategory(C4D_StaticBack);
	SetClrModulation(RGBa(255,255,255,255));
	this.Collectible = false;
	return _inherited();
}

func Destruct()
{
	SetCategory(C4D_Object);
	SetClrModulation(RGBa(180, 180, 180, 255));
	this.Collectible = true;
	return _inherited();
}

func OnHitByPickaxe()
{
	Destruct();
}

func PreviewBuildingCondition(object caller)
{
	if (BuildingCondition())
		return true;
	
	if (VerticesStuck() == GetVertexNum()+1)
		return false;
	
	for(var obj in FindObjects(Find_Exclude(this), Find_Or(Find_OnLine(-GetObjWidth()/2-1, 0, GetObjWidth()/2+1, 0), Find_OnLine(0, -GetObjHeight()/2-1, 0, GetObjHeight()/2+2))
			,Find_Func("IsPreview"), Find_ID(GetID()), Find_Owner(GetController())))
	{
		if(obj == caller)
			continue;
		
		if(obj->PreviewBuildingCondition(this))
			return true;
	}
	
	return false;
}