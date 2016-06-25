/**
	Lib_BuildingElement
	

	@author KKenny
*/

#include Lib_BuildingTile

local Name = "$Name$";
local Description = "$Description$";
local Plane = 200;

local tile_mode = TILE_MODE_LINE;

local HitPoints = 50;

func IsSolidBuildingTile() { return true; }

func Constructed()
{
	SetSolidMask(0,0,tile_size_x,tile_size_y);
	AdjustSurroundingMaterial(true, true, true, true);
	return _inherited();
}

func PreviewBuildingCondition(caller)
{
	if (FindObject(Find_AtRect(-tile_size_x/2, -tile_size_y/2, tile_size_x, tile_size_y), 
		Find_Exclude(this), 
		Find_Not(Find_Category(C4D_Object)),
		Find_Not(Find_Func("IsPreview")), 
		Find_Not(Find_Func("IsWallBuildingTile"))))
		return false;
		
	return _inherited(caller);
}

func BuildingCondition()
{
	if (VerticesStuckSemi() == GetVertexNum()+1)
		return false;
	
	if (FindObject(Find_AtRect(-tile_size_x/2, -tile_size_y/2, tile_size_x, tile_size_y), 
		Find_Exclude(this), 
		Find_Not(Find_Category(C4D_Object)),
		Find_Not(Find_Func("IsPreview")), 
		Find_Not(Find_Func("IsWallBuildingTile"))))
		return false;

	if ((GBackSolid(-(tile_size_x), 0) || GBackSolid(tile_size_x, 0) || GBackSolid(0, (tile_size_y)) || GBackSolid(0,-(tile_size_y))))
		return true;
	
	if (FindObject(Find_Exclude(this), Find_Not(Find_Func("IsPreview")), Find_Func("IsSolidBuildingTile"), Find_Or(Find_OnLine(-tile_size_x/2-1, 0, tile_size_x/2+1, 0), Find_OnLine(0, -tile_size_y/2-1, 0, tile_size_y/2+2))))
		return true;
	
	if (FindObject(Find_AtPoint(0, tile_size_y), Find_Func("IsPillarBuildingTile")))
		return true;
	
	return false;
}

func Destruct()
{
	SetSolidMask();
	_inherited();
	OnBecomeUnstable();
}

private func Destroy()
{
	SetCategory(C4D_None);
	SetSolidMask();
	OnBecomeUnstable();
	
	var particles = 
	{
		Size = PV_KeyFrames(0, 0, 0, 100, PV_Random(3, 5), 1000, 3),
		R = PV_Random(100, 255, nil, 1), G = PV_Random(100, 255, nil, 1), B = PV_Random(100, 255, nil, 1),
		Alpha = PV_Linear(255, 0),
		ForceY = PV_Gravity(100),
		CollisionVertex = 0
	};
	CreateParticle("SmokeDirty", PV_Random(-build_grid_x, +build_grid_x), PV_Random(-build_grid_y, +build_grid_y), PV_Random(-10, 10), PV_Random(-10, 10), PV_Random(10, 60), particles, 50);
	Sound("Hits::Materials::Rock::Rockfall*");
	RemoveObject();
} 

public func OnBecomeUnstable()
{
	var pillar = FindObject(Find_AtPoint(0, -build_grid_y), Find_Category(C4D_StaticBack), Find_Func("IsPillarBuildingTile"));
	if (pillar) pillar->Destroy();
	
	if (this && this.no_propagation) return;
	for (var neighbour in GetNeighbours())
		if (neighbour) neighbour->CheckSupport();
}

public func CheckSupport()
{
	this.already_found = true;
	var has_support = false;
	var neighbour_tiles = [this];
	for (var i = 0; i < GetLength(neighbour_tiles); ++i)
	{
		var current = neighbour_tiles[i];
		if (!current) continue;
		
		for (var neighbour in current->GetNeighbours(true))
		{
			neighbour.already_found = true;
			//if (ObjectDistance(this, neighbour) < 100)
			PushBack(neighbour_tiles, neighbour);
			
			if ((neighbour->GBackSolid(0, build_grid_y) && (neighbour->GetMaterial(0, build_grid_y) != Material("Vehicle")))
			|| (neighbour->FindObject(Find_AtPoint(0, build_grid_y), Find_Category(C4D_StaticBack), Find_Func("IsPillarBuildingTile"))))
			{
				has_support = true;
				break;
			} 
		}
		
		if (has_support) break;
	}
	
	for (var neighbour in neighbour_tiles)
	{
		if (!neighbour) continue;
		
		if (!has_support)
		{
			neighbour.no_propagation = true;
			neighbour->Destroy();
		}
		else
		{
			neighbour.already_found = nil;
		}
	}
}

/*-- Damage Handling --*/

public func GetHitPoints()
{
	return this.HitPoints;
}

public func GetRemainingHitPoints()
{
	return this.HitPoints - GetDamage();
}

public func Damage(int change, int cause, int cause_plr)
{
	if (!this.take_direct_damage)
	{
		var deminishing_damage = change / 2;
		if (GetDamage() > this.HitPoints)
			deminishing_damage += (GetDamage() - this.HitPoints) * 2;
		this.already_found = true;
		var neighbour_tiles = [this];
		for (var i = 0; i < GetLength(neighbour_tiles); ++i)
		{
			if (deminishing_damage <= 0) break;
			
			var current = neighbour_tiles[i];
			if (!current) continue;
			
			for (var neighbour in current->GetNeighbours(true))
			{
				neighbour.already_found = true;;
				PushBack(neighbour_tiles, neighbour);
			}
			
			if (i == 0) continue;
			
			current.take_direct_damage = true;
			current->DoDamage(deminishing_damage, cause, cause_plr);
			if (current)
				current.take_direct_damage = false;
			
			deminishing_damage -= 4;
			
			// A whole column might have been collapsed (including myself).
			if (!this) break;
		}
		
		for (var neighbour in neighbour_tiles)
		{
			if (neighbour) neighbour.already_found = nil;
		}
	}
	
	if (!this) return;
	if (GetDamage() > this.HitPoints)
	{
		Destroy();
	}
	if (this)
		UpdateDamageDisplay();
}

private func GetNeighbours(bool ignore_cycles)
{
	var blocks = [];
	var x_pos = [-1, +1, 0, 0];
	var y_pos = [0, 0, -1, 1];
	for (var i = 0; i < 4; ++i)
	{
		var block = FindObject(Find_AtPoint(x_pos[i] * build_grid_x, y_pos[i] * build_grid_y), Find_Func("IsSolidBuildingTile"), Find_Category(C4D_StaticBack));
		if (!block) continue;
		if (block.already_found && ignore_cycles) continue;
		PushBack(blocks, block);
	}
	return blocks;
}

private func UpdateDamageDisplay()
{
	ClearParticles();
	var particles = 
	{
		R = PV_Random(0, 50), G = PV_Random(0, 50), B = PV_Random(0, 50),
		Rotation = PV_Random(0, 360),
		Stretch = PV_Random(0, 4000),
		Size = PV_Random(0, 3),
		Attach = ATTACH_Front | ATTACH_MoveRelative
	};
	CreateParticle("Fire", PV_Random(-4, 4), PV_Random(-4, 4), 0, 0, 0, particles, 3 * GetDamage() / 2);
}
