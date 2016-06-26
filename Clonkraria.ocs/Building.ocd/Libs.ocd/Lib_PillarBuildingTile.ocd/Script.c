/**
	Lib_PillarBuildingTile
	

	@author 
*/

#include Lib_BuildingTile

local Name = "$Name$";
local Description = "$Description$";


local tile_mode = TILE_MODE_VERTICAL_LINE;
local Plane = 10;

func IsPillarBuildingTile() { return true; }

func Constructed()
{
	AdjustSurroundingMaterial(true, false, false, false);
	//SetObjDrawTransform(1000,0,0,0,1200);
	return _inherited();
}

func BuildingCondition()
{
	if (FindObject(Find_Not(Find_Func("IsPreview")), Find_AtPoint(), Find_Not(Find_Func("IsWallBuildingTile")), Find_Func("IsBuildingTile"), Find_Exclude(this)))
		return false;

	if (VerticesStuckSemi() == GetVertexNum()+1)
		return false;
	
	if (FindObject(Find_AtPoint(0, tile_size_y), Find_Not(Find_Func("IsPreview")), Find_Or(Find_Func("IsSolidBuildingTile"), Find_Func("IsPillarBuildingTile"))) 
		|| GBackSolid(0, (tile_size_y)))
		return true;
	
	return false;
}

func Destruct()
{
	_inherited();
	SetObjDrawTransform(1000,0,0,0,1000);
	OnBecomeUnstable();
}

private func Destroy()
{
	SetCategory(C4D_None);
	OnBecomeUnstable();
	
	var particles = 
	{
		Size = PV_KeyFrames(0, 0, 0, 100, PV_Random(0, 2), 1000, 3),
		R = PV_Random(100, 150, nil, 1), G = PV_Random(100, 150, nil, 1), B = PV_Random(100, 150, nil, 1),
		Alpha = PV_Linear(255, 0),
		ForceY = PV_Gravity(200),
		CollisionVertex = 0,
		Stretch = PV_Random(2000, 4000),
		Rotation = PV_Random(-10, 10)
	};
	CreateParticle("SmokeDirty", PV_Random(-build_grid_x, +build_grid_x), PV_Random(-build_grid_y, +build_grid_y), PV_Random(-15, 15), PV_Random(-2, 2), PV_Random(10, 30), particles, 50);
	Sound("Hits::Materials::Rock::Rockfall*");
	RemoveObject();
} 

public func OnBecomeUnstable()
{
	if (this && this.no_propagation) return;
	
	var position_y = -build_grid_y;
	while (true)
	{
		var tile = FindObject(Find_AtPoint(0, position_y), Find_Category(C4D_StaticBack), Find_Func("IsPillarBuildingTile"));
		if (tile)
		{
			tile.no_propagation = true;
			tile->Destroy();
		}
		else
		{
			tile = FindObject(Find_AtPoint(0, position_y), Find_Category(C4D_StaticBack), Find_Func("IsSolidBuildingTile"));
			if (tile) tile->OnBecomeUnstable();
			break;
		}
		position_y -= build_grid_y;
	}
}
