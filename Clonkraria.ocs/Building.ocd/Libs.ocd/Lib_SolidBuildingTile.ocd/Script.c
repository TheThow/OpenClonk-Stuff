/**
	Lib_BuildingElement
	

	@author KKenny
*/

#include Lib_BuildingTile

local Name = "$Name$";
local Description = "$Description$";
local Plane = 300;

local tile_mode = TILE_MODE_LINE;

func IsSolidBuildingTile() { return true; }

func Constructed()
{
	SetSolidMask(0,0,GetObjWidth(),GetObjHeight());
	
	for (var x = -GetObjWidth()/2; x < GetObjWidth()/2; x++)
	{
		var mat = nil;
		for(var y = GetObjHeight(); y > GetObjHeight()/2-1; y--)
		{
			if (!GBackSolid(x, y) && mat == nil)
				break;
			
			if (!GBackSolid(x, y))
				DrawMaterialQuad(MaterialName(mat), GetX() + x, GetY() + y, GetX() + x + 1, GetY() + y, GetX() + x + 1, GetY() + y + 1, GetX() + x, GetY() + y + 1);
			
			mat = GetMaterial(x,y);
		}
	}
	
	return _inherited();
}

func BuildingCondition()
{
	if(GBackSolid())
		return false;
	
	if (FindObject(Find_AtRect(-GetObjWidth()/2, -GetObjHeight()/2, GetObjWidth(), GetObjHeight()), Find_Exclude(this), Find_Not(Find_Func("IsPreview")), Find_Not(Find_Func("IsWallBuildingTile"))))
		return false;

	if ((GBackSolid(-(GetObjWidth()), 0) || GBackSolid(GetObjWidth(), 0) || GBackSolid(0, (GetObjHeight())) || GBackSolid(0,-(GetObjHeight()))))
		return true;
	
	if (FindObject(Find_Exclude(this), Find_Not(Find_Func("IsPreview")), Find_Func("IsSolidBuildingTile"), Find_Or(Find_OnLine(-GetObjWidth()/2-1, 0, GetObjWidth()/2+1, 0), Find_OnLine(0, -GetObjHeight()/2-1, 0, GetObjHeight()/2+2))))
		return true;
	
	return false;
}

func Destruct()
{
	SetSolidMask();
	return _inherited();
}