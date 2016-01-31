/**
	Light Ray
	

	@author 
*/

local ManaCost = 25;

local SpellDamage = 15;
local Speed = 25;
local Durr = 35;

local Size = 15;

local Name = "$Name$";
local Description = "$Description$";

local LifeTime = 40;
local current_length = 0;
local angle = 0;
local shooter;
func Initialize()
{
	SetCategory(C4D_StaticBack);
}

func Launch(object clonk, int x, int y)
{
	shooter = clonk;
	SetController(clonk->GetOwner());
	angle = Angle(0, 0, x, y);
	AddEffect("HitCheck", this, 1,1, nil,nil, clonk, true);
	ScheduleCall(this, "Hit", LifeTime);
	
	AddEffect("LightRay", this, 1, 1, this, nil);
	Sound("Structures::FanLoop", nil, nil, nil, 1, nil, 200);
}

func Hit()
{
	RemoveObject();
}

public func HitObject(obj)
{
	if(obj->~CanBeHit() == false)
		return;

	obj->DoEnergy(-SpellDamage * 100, true, nil, GetController());
}

func FxLightRayStart(target, fx, temp)
{
	if (temp) return;
	fx.stars = 
	{
		Stretch = PV_Random(15000, 20000),
		Size = PV_Linear(4, 0),
		Alpha = PV_Random(255,0,3),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Random(0, 360),
		Attach = ATTACH_MoveRelative
	};
	
	fx.ray = 
	{
		Alpha = PV_Linear(255,0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = angle,
		Attach = ATTACH_MoveRelative,
	};
}

func FxLightRayTimer(target, fx, time)
{
	if (!shooter) return -1;
	current_length += 5;
	var new_coords = GetPlayerCursorPos(shooter->GetOwner(), true);
	if (new_coords)
		angle = Angle(shooter->GetX(), shooter->GetY(), new_coords[0], new_coords[1]);
	var from_x = AbsX(shooter->GetX());
	var from_y = AbsY(shooter->GetY());
	var to_x   = from_x + Sin(angle, current_length);
	var to_y   = from_y - Cos(angle, current_length);
	var point  = PathFree2(GetX() + from_x, GetY() + from_y, GetX() + to_x, GetY() + to_y);
	var len = current_length;
	if (point)
	{
		to_x = point[0] - GetX();
		to_y = point[1] - GetY();
		len = Distance(from_x, from_y, to_x, to_y);
	}
	var mid_x  = (from_x + to_x) / 2;
	var mid_y  = (from_y + to_y) / 2;
	
	fx.ray.Size = len;
	fx.ray.Rotation = angle;
	
	CreateParticle("StarSpark", from_x, from_y, 0, 0, 10, fx.stars, 2);
	CreateParticle("StarSpark", to_x, to_y, 0, 0, 10, fx.stars, 2);
	CreateParticle("RaySpark", mid_x, mid_y, 0, 0, 10, fx.ray, 1);
	for (var obj in FindObjects(Find_OnLine(from_x, from_y, to_x, to_y), Find_OCF(OCF_Alive), Find_Exclude(shooter), Find_NoContainer()))
	{
		CreateParticle("StarSpark", AbsX(obj->GetX()), AbsY(obj->GetY()), 0, 0, 10, fx.stars, 2);
		HitObject(obj);
	}
}

