/**
	Lib_BuildingElement
	

	@author KKenny
*/

local Name = "$Name$";
local Description = "$Description$";

local build_offset_x = 0;
local build_offset_y = 0;

local build_grid_x = 8;
local build_grid_y = 8;

local preview_object;

func IsBuildingElement() { return true; }
func HoldingEnabled() { return true; }

func PreviewMode()
{
	SetCategory(C4D_StaticBack);
	SetClrModulation(RGBa(255, 255, 255, 128));
	this.Visibility = VIS_Owner;
	this.Collectible = false;
	AddEffect("Preview", this, 1, 1, this);
}

func FxPreviewTimer()
{
	if(BuildingCondition())
		SetClrModulation(RGBa(255, 255, 255, 128));
	else
		SetClrModulation(RGBa(255, 0, 0, 128));
}

public func ControlUseStart(object clonk, int x, int y)
{
	preview_object = CreateObject(GetID(), 0, 0, clonk->GetOwner());
	preview_object.Collectible = 0;
	preview_object->PreviewMode();
	ControlUseHolding(clonk, x, y);
	
	return true;
}

public func ControlUseHolding(object clonk, int x, int y)
{
	var tx = GetRightX(clonk->GetX() + x);
	var ty = GetRightY(clonk->GetY() + y);
	preview_object->SetPosition(tx, ty);
	return true;
}

public func ControlUseStop(object clonk, int x, int y)
{
	if(preview_object->BuildingCondition())
	{
		var obj = this->TakeObject();
		obj->SetPosition(preview_object->GetX(), preview_object->GetY());
		obj->Constructed();
	}
	
	//preview_object->RemoveObject();
	return true;
}

public func ControlUseCancel(object clonk, int x, int y)
{
	if(preview_object)
		preview_object->RemoveObject();
	return true;
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