/**
	HookPoint
	

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local points;
local pid;
local pspeed;

func Set(angle, step, proid, speed)
{
	var fx = AddEffect("Launch", this, 1, step, this);
	fx.angle = angle;
	pid = proid;
	pspeed = speed;
}

func FxLaunchTimer(target, fx)
{
	var o = CreateObject(pid, 0, 0, -1);
	var x = Cos(fx.angle, 100);
	var y = Sin(fx.angle, 100);
	if(pspeed != nil)
		o->SetVelocity(fx.angle, pspeed);
	o->Launch(this, x, y);
}