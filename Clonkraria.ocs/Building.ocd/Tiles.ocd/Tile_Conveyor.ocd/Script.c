/**
	Tile_Conveyor
	

	@author 
*/

#include Lib_BuildingTile

local Name = "$Name$";
local Description = "$Description$";
local Collectible = 1;
local Plane = 10;

local tile_mode = TILE_MODE_LINE;

local IsConveyorBuildingTile = true;

local last_neighbours = [[nil, nil, nil], [nil, nil, nil], [nil, nil, nil]];

protected func Hit(x, y)
{
	StonyObjectHit(x,y);
	return true;
}

func BuildingCondition()
{
	if (FindObject(Find_AtPoint(), Find_Not(Find_Func("IsPreview")), Find_Not(Find_Or(Find_Func("IsWallBuildingTile"), Find_Func("IsPillarBuildingTile"))), Find_Func("IsBuildingTile"), Find_Exclude(this)))
		return false;

	if (VerticesStuckSemi() == GetVertexNum()+1)
		return false;
	
	if (!FindObject(Find_Distance(3*tile_size_y), Find_Not(Find_Func("IsPreview")), Find_Or(Find_Func("IsSolidBuildingTile"), Find_Func("IsPillarBuildingTile")))) 
		return false;
	
	return true;
}

func Constructed()
{
	_inherited();
	
	var neighbours = GetNeighboursAsMatrix();
	for (var axis in neighbours)
		for (var obj in axis)
			if (obj) obj->UpdateGraphics();
	UpdateGraphics(neighbours);
	
	AddTimer("CheckObjects", 60 + Random(20));
}

public func Destroy()
{
	RemoveTimer("CheckObjects");
	return inherited(...);
}

private func UpdateGraphics(array neighbours)
{
	neighbours = neighbours || GetNeighboursAsMatrix();
	
	var count = 0;
	for (var axis in neighbours)
		for (var obj in axis)
			if (obj) ++count;
	if (count >= 3 || count == 1)
	{
		SetGraphics("");
	}
	else
	if (count == 2)
	{
		var horizontal = (neighbours[0][1] && neighbours[2][1]);
		var vertical = (neighbours[1][0] && neighbours[1][2]);
		if (vertical)
		{
			SetGraphics("Straight");
			SetR(0);
		}
		else if (horizontal)
		{
			SetGraphics("Straight");
			SetR(90);
		}
		else
		{
			SetGraphics("Edge");
			if (neighbours[1][0] && neighbours[2][1])
				SetR(0);
			else if (neighbours[2][1] && neighbours[1][2])
				SetR(90);
			else if (neighbours[1][2] && neighbours[0][1])
				SetR(180);
			else
				SetR(270);
		}
	}
}

private func GetNeighboursAsMatrix(bool ignore_cycles)
{
	var blocks = [[nil, nil, nil], [nil, nil, nil], [nil, nil, nil]];
	var x_pos = [-1, +1, 0, 0];
	var y_pos = [0, 0, -1, +1];
	for (var i = 0; i < 4; ++i)
	{
		var block = FindObject(Find_AtPoint(x_pos[i] * build_grid_x, y_pos[i] * build_grid_y), Find_Property("IsConveyorBuildingTile"), Find_Category(C4D_StaticBack));
		if (!block) continue;
		if (block.already_found && ignore_cycles) continue;
		blocks[1 + x_pos[i]][1 + y_pos[i]] = block;
	}
	
	last_neighbours = blocks;
	return blocks;
}

private func GetNeighbours(bool ignore_cycles)
{
	var blocks = [];
	var x_pos = [-1, +1, 0, 0];
	var y_pos = [0, 0, -1, +1];
	for (var i = 0; i < 4; ++i)
	{
		var block = FindObject(Find_AtPoint(x_pos[i] * build_grid_x, y_pos[i] * build_grid_y), Find_Property("IsConveyorBuildingTile"), Find_Category(C4D_StaticBack));
		if (!block) continue;
		if (block.already_found && ignore_cycles) continue;
		PushBack(blocks, block);
	}
	return blocks;
}

private func CheckObjects()
{
	var rect = Find_AtRect(-build_grid_x / 2, -build_grid_y / 2, build_grid_x, build_grid_y * 4);
	var obj = FindObject(rect, Find_Category(C4D_Object), 
		Find_Not(Find_OCF(OCF_HitSpeed1)), Find_NoContainer(), Find_Not(Find_Property("is_being_moved_by_conveyor")),
		Find_PathFree(this)
		);
	if (!obj) return;
	if (FindObject(rect, Find_ID(Tile_ConveyorArmHook))) return;
	
	BeginTransport(obj);
}

private func BeginTransport(object obj)
{
	var base = CreateObject(Tile_ConveyorArmBase, obj->GetX() - GetX(), 0, NO_OWNER);
	base.hook->MoveToGrab(obj);
	base.home_block = this;
}

public func IsTarget(object origin, object payload)
{
	if (FindObject(Find_OnLine(0, 0, 0, 40), Find_ID(Lorry), Find_PathFree(this))) return true;
	return false;
}

local Components = {Wood = 1};
public func IsToolProduct() { return true; }
