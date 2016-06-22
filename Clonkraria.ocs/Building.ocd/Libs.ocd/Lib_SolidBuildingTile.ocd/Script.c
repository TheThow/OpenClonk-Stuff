/**
	Lib_BuildingElement
	

	@author KKenny
*/

#include Lib_BuildingTile

local Name = "$Name$";
local Description = "$Description$";


func IsSolidBuildingTile() { return true; }

func Constructed()
{
	SetSolidMask(0,0,8,8);
	
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
	
	if (FindObject(Find_AtRect(-GetObjWidth()/2, -GetObjHeight()/2, GetObjWidth(), GetObjHeight()), Find_Exclude(this)))
		return false;

	if ((GBackSolid(-(GetObjWidth()), 0) || GBackSolid(GetObjWidth(), 0) || GBackSolid(0, (GetObjHeight())) || GBackSolid(0,-(GetObjHeight()))))
		return true;
	
	if (FindObject(Find_Exclude(this), Find_Func("IsSolidBuildingTile"), Find_Or(Find_AtRect(-GetObjWidth()-1, -2, GetObjWidth()+1, 4), Find_InRect(-2, -GetObjHeight()-1, 4, GetObjHeight()+1))))
		return true;
	
	return false;
}

func Destruct()
{
	SetSolidMask();
	return _inherited();
}