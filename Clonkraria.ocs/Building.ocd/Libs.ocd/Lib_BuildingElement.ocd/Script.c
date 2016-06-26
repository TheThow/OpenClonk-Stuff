/**
	Lib_BuildingElement
	

	@author KKenny
*/

local Name = "$Name$";
local Description = "$Description$";

local build_offset_x = 0;
local build_offset_y = 0;

local build_grid_x = 10;
local build_grid_y = 10;

local starting_point_x;
local starting_point_y;

local preview_objects;

static const TILE_MODE_SINGLE = 1;
static const TILE_MODE_LINE = 2;
static const TILE_MODE_VERTICAL_LINE = 3;
static const TILE_MODE_RECTANGLE = 4;
local tile_mode = TILE_MODE_SINGLE;

func IsBuildingElement() { return true; }
func HoldingEnabled() { return true; }
func IsPreview() { if(GetEffect("Preview", this)) return true; return false; }

public func Construction()
{
	preview_objects = [];
}

func PreviewMode()
{
	SetCategory(C4D_StaticBack);
	SetClrModulation(RGBa(255, 255, 255, 128));
	this.Visibility = VIS_Owner;
	AddEffect("Preview", this, 1, 1, this);
}

func FxPreviewTimer()
{
	if(PreviewBuildingCondition([this]))
		SetClrModulation(RGBa(255, 255, 255, 128));
	else
		SetClrModulation(RGBa(255, 0, 0, 128));
}

public func ControlUseStart(object clonk, int x, int y)
{
	starting_point_x = GetRightX(clonk->GetX() + x);
	starting_point_y = GetRightY(clonk->GetY() + y);
	
	SetController(clonk->GetOwner());
	CreatePreviewObject();
	ControlUseHolding(clonk, x, y);
	
	return true;
}

public func ControlUseHolding(object clonk, int x, int y)
{
	var tx = GetRightX(clonk->GetX() + x);
	var ty = GetRightY(clonk->GetY() + y);
	if (tile_mode == TILE_MODE_SINGLE)
		preview_objects[0]->SetPosition(tx, ty);
	else if (tile_mode == TILE_MODE_LINE)
		DrawPreviewLine(starting_point_x, starting_point_y, clonk->GetX() + x, clonk->GetY() + y);
	else if (tile_mode == TILE_MODE_VERTICAL_LINE)
		DrawPreviewLine(starting_point_x, starting_point_y, starting_point_x, clonk->GetY() + y);
	return true;
}

public func ControlUseStop(object clonk, int x, int y)
{
	for (var preview_object in preview_objects)
	{
		if (!preview_object) continue;
		if (!preview_object->BuildingCondition()) continue;
		var obj = this->TakeObject();
		obj->SetPosition(preview_object->GetX(), preview_object->GetY());
		obj->Constructed();
		obj.Collectible = 0;
	}
	
	ClearPreviewObjects();
	return true;
}

public func ControlUseCancel(object clonk, int x, int y)
{
	/*if(preview_object)
		preview_object->RemoveObject();*/
	AddEffect("RemovePreview", this, 1, 1, this);
	return true;
}

private func DrawPreviewLine(int from_x, int from_y, int to_x, int to_y)
{
	var len_x = to_x - from_x;
	var len_y = to_y - from_y;
	var len = Max(Abs(len_x), Abs(len_y));
	var xdir = 0;
	var ydir = 0;
	
	if (Abs(len_x) > Abs(len_y)) xdir = BoundBy(len_x, -1, 1);
	else ydir = BoundBy(len_y, -1, 1);
	
	// Round up.
	var blocks = len / build_grid_x + 1;
	
	var x = starting_point_x;
	var y = starting_point_y;
	for (var i = 0; i < blocks; ++i)
	{
		var block;
		if (i >= GetLength(preview_objects))
			block = CreatePreviewObject();
		else block = preview_objects[i];
		if (!block) continue;
		
		block->SetPosition(x, y);
				
		x += xdir * build_grid_x;
		y += ydir * build_grid_y;
	}
	
	var original_blocks = blocks;
	while (blocks < GetLength(preview_objects))
	{
		if (preview_objects[blocks])
			preview_objects[blocks]->RemoveObject();
		blocks++;
	}
	SetLength(preview_objects, original_blocks);
}


private func CreatePreviewObject()
{
	var preview_object = CreateObject(GetID(), 0, 0, GetController());
	preview_object.Collectible = 0;
	preview_object->PreviewMode();
	PushBack(preview_objects, preview_object);
	return preview_object;
}

private func ClearPreviewObjects()
{
	for (var obj in preview_objects)
		if (obj) obj->RemoveObject();
	preview_objects = [];
}

//Workaround
func FxRemovePreviewStop()
{
	ClearPreviewObjects();
}

func Constructed()
{
	return true;
}

func Destruct()
{
	return true;
}

func Destroy()
{
	return true;
}

func BuildingCondition()
{
	return true;
}

func PreviewBuildingCondition(callers)
{
	return BuildingCondition();
}

func GetRightX(int x)
{
	var off = x % build_grid_x;
	x = x - off;
	
	if (off > build_grid_x/2)
		off = build_grid_x;
	else
		off = 0;
	
	return build_offset_x + x + off;
}

func GetRightY(int y)
{
	var off = y % build_grid_y;
	y = y - off;
	
	if (off > build_grid_y/2)
		off = build_grid_y;
	else
		off = 0;
	
	return build_offset_y + y + off;
}