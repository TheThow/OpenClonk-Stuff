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
		
		mat = nil;
		for(var y = -GetObjHeight(); y < -GetObjHeight()/2+1; y++)
		{
			if (!GBackSolid(x, y) && mat == nil)
				break;
			
			if (!GBackSolid(x, y))
				DrawMaterialQuad(MaterialName(mat), GetX() + x, GetY() + y, GetX() + x + 1, GetY() + y, GetX() + x + 1, GetY() + y + 1, GetX() + x, GetY() + y + 1);
			
			mat = GetMaterial(x,y);
		}
	}
	
	
	for (var y = -GetObjHeight()/2; y < GetObjHeight()/2; y++)
	{
		var mat = nil;
		for(var x = -GetObjWidth(); x < -GetObjWidth()/2+1; x++)
		{
			if (!GBackSolid(x, y) && mat == nil)
				break;
			
			if (!GBackSolid(x, y))
				DrawMaterialQuad(MaterialName(mat), GetX() + x, GetY() + y, GetX() + x + 1, GetY() + y, GetX() + x + 1, GetY() + y + 1, GetX() + x, GetY() + y + 1);
			
			mat = GetMaterial(x,y);
		}
		
		mat = nil;
		for(var x = GetObjWidth(); x > GetObjWidth()/2-1; x--)
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
	if (VerticesStuckSemi() == GetVertexNum()+1)
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

private func Destroy()
{
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
		this.damage_cycle = FrameCounter();
		var neighbour_tiles = [this];
		for (var i = 0; i < GetLength(neighbour_tiles); ++i)
		{
			if (deminishing_damage <= 0) break;
			
			var current = neighbour_tiles[i];
			if (!current) continue;
			
			for (var neighbour in current->GetNeighbours(this.damage_cycle))
			{
				neighbour.damage_cycle = this.damage_cycle;
				PushBack(neighbour_tiles, neighbour);
			}
			
			if (i == 0) continue;
			
			current.take_direct_damage = true;
			current->DoDamage(deminishing_damage, cause, cause_plr);
			if (current)
				current.take_direct_damage = false;
			
			deminishing_damage -= 5;
		}
		
		for (var neighbour in neighbour_tiles)
		{
			if (neighbour) neighbour.damage_cycle = nil;
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

private func GetNeighbours(ignore_damage_cycle)
{
	var blocks = [];
	var x_pos = [-1, -1, 0, 0];
	var y_pos = [0, 0, -1, 1];
	for (var i = 0; i < 4; ++i)
	{
		var block = FindObject(Find_AtPoint(x_pos[i] * build_grid_x, y_pos[i] * build_grid_y), Find_Func("IsSolidBuildingTile"));
		if (!block) continue;
		if (block.damage_cycle != nil && block.damage_cycle == ignore_damage_cycle) continue;
		PushBack(blocks, block);
	}
	return blocks;
}

private func UpdateDamageDisplay()
{
	ClearParticles("Fire", this);
	var particles = 
	{
		R = PV_Random(0, 50), G = PV_Random(0, 50), B = PV_Random(0, 50),
		Rotation = PV_Random(0, 360),
		Stretch = PV_Random(0, 3000),
		Size = PV_Random(0, 2),
		Attach = ATTACH_Front | ATTACH_MoveRelative
	};
	CreateParticle("Fire", PV_Random(-3, 3), PV_Random(-3, 3), 0, 0, 0, particles, 3 * GetDamage() / 2);
}
