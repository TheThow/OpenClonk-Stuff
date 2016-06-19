/**
	HookPoint
	

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local points;

func Initialize()
{
	points = [];
	
	SetLightRange(30, 70);
	SetLightColor(RGB(255, 0, 255));
	
	var props = {
		Size = 40,
		Attach = ATTACH_Back|ATTACH_MoveRelative,
		R = 255,
		G = 0,
		B = 255,
		Alpha = 60
	};
	
	CreateParticle("Flash", 0, 0, 0, 0, 0, props, 1);

	for(var i = 0; i < 360; i+= 90)
	{
		var obj = CreateObject(Amethyst, Cos(i, 6), Sin(i, 6), -1);
		obj->SetCategory(C4D_StaticBack);
		PushBack(points, obj);
	}
}
