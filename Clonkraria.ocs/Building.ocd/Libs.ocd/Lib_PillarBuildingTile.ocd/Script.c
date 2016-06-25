/**
	Lib_PillarBuildingTile
	

	@author 
*/

#include Lib_BuildingTile

local Name = "$Name$";
local Description = "$Description$";


local tile_mode = TILE_MODE_VERTICAL_LINE;


func IsPillarBuildingTile() { return true; }

func BuildingCondition()
{
	if (FindObject(Find_AtPoint(), Find_Func("IsPillarBuildingTile"), Find_Exclude(this)))
		return false;

	if (VerticesStuckSemi() == GetVertexNum()+1)
		return false;
	
	if (!FindObject(Find_AtPoint(0, GetObjHeight()), Find_Or(Find_Func("IsSolidBuildingTile"), Find_Func("IsPillarBuildingTile"))))
		return false;

	if (FindObject(Find_Exclude(this), Find_Func("IsWallBuildingTile"), Find_Not(Find_Func("IsPreview")),
		Find_AtPoint()))
		//Find_Or(Find_AtRect(-GetObjWidth()/2-1, -GetObjHeight()/2, GetObjWidth()+2, GetObjHeight()), Find_AtRect(-GetObjWidth()/2, -GetObjHeight()/2-1, GetObjWidth(), GetObjHeight()+2))))
		return true;
	
	return false;
}

func Destruct()
{
	_inherited();
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
