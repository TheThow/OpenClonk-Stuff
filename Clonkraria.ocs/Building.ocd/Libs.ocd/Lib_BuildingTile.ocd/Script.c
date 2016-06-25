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

local tile_size_x = 10;
local tile_size_y = 10;

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

func PreviewBuildingCondition(callers)
{
	if (BuildingCondition())
		return true;
	
	if (VerticesStuck() == GetVertexNum()+1)
		return false;
	
	for(var obj in FindObjects(Find_Exclude(this), Find_Or(Find_OnLine(-tile_size_x/2-1, 0, tile_size_x/2+1, 0), Find_OnLine(0, -tile_size_y/2-1, 0, tile_size_y/2+2))
			,Find_Func("IsPreview"), Find_ID(GetID()), Find_Owner(GetController())))
	{
		
		var flag = 0;
		for(var caller in callers)
		{
			if(obj == caller)
			{
				flag = 1;
				break;
			}
		}
		
		if (flag)
			continue;
		
		PushBack(callers, this);
		
		if(obj->PreviewBuildingCondition(callers))
			return true;
	}
	
	return false;
}

func AdjustSurroundingMaterial(up, down, left, right)
{
	for (var x = -tile_size_x/2; x < tile_size_x/2; x++)
	{
		var mat = nil;
		if(up)
		{
			for(var y = tile_size_y; y > tile_size_y/2-1; y--)
			{
				if (!GBackSolid(x, y) && mat == nil)
					break;
				
				if (!GBackSolid(x, y))
					DrawMaterialQuad(MaterialName(mat), GetX() + x, GetY() + y, GetX() + x + 1, GetY() + y, GetX() + x + 1, GetY() + y + 1, GetX() + x, GetY() + y + 1);
				
				mat = GetMaterial(x,y);
			}
		}
		
		if(down)
		{
			mat = nil;
			for(var y = -tile_size_y; y < -tile_size_y/2; y++)
			{
				if (!GBackSolid(x, y) && mat == nil)
					break;
				
				if (!GBackSolid(x, y))
					DrawMaterialQuad(MaterialName(mat), GetX() + x, GetY() + y, GetX() + x + 1, GetY() + y, GetX() + x + 1, GetY() + y + 1, GetX() + x, GetY() + y + 1);
				
				mat = GetMaterial(x,y);
			}
		}
	}
	
	
	for (var y = -tile_size_y/2; y < tile_size_y/2; y++)
	{
		var mat = nil;
		if(left)
		{
			for(var x = -tile_size_x; x < -tile_size_x/2+1; x++)
			{
				if (!GBackSolid(x, y) && mat == nil)
					break;
				
				if (!GBackSolid(x, y))
					DrawMaterialQuad(MaterialName(mat), GetX() + x, GetY() + y, GetX() + x + 1, GetY() + y, GetX() + x + 1, GetY() + y + 1, GetX() + x, GetY() + y + 1);
				
				mat = GetMaterial(x,y);
			}
		}
		
		if(right)
		{
			mat = nil;
			for(var x = tile_size_x; x > tile_size_x/2-1; x--)
			{
				if (!GBackSolid(x, y) && mat == nil)
					break;
				
				if (!GBackSolid(x, y))
					DrawMaterialQuad(MaterialName(mat), GetX() + x, GetY() + y, GetX() + x + 1, GetY() + y, GetX() + x + 1, GetY() + y + 1, GetX() + x, GetY() + y + 1);
				
				mat = GetMaterial(x,y);
			}
		}
	}
}