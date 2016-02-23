/**
	SlowField
	

	@author 
*/

local ManaCost = 40;
local SpellRange = 275;


local MaxSize = 65;
local currentSize;

local Dur = 120;

local r;

func Initialize()
{
	currentSize = 0;
}

func CenterParticle()
{
	return {
		Size = MaxSize*2,
		BlitMode = GFX_BLIT_Additive,
		R = 255,
		G = 165,
		B = 50,
		Alpha=50,
		Attach=ATTACH_Back,
	};
}

func Launch(object clonk, int x, int y)
{
	AddEffect("Grow", this, 1, 1, this);
	AddEffect("Slow", this, 1, 1, this);
	AddEffect("Remove", this, 1, Dur, this);
	Sound("slowfield", false, 100);
}

func FxGrowTimer(object target, proplist effect, int time)
{
	if(currentSize >= MaxSize)
		return -1;

	var growcenter =
	{
		Prototype = CenterParticle(),
		Size = currentSize*2,
		Alpha = PV_Linear(255,0)
	};
	
	CreateParticle("Flash", 0, 0, 0, 0, 1, growcenter, 1);
	
	DrawBorder();
	
	if(currentSize < MaxSize)
		currentSize+=3;
}

func DrawBorder()
{
/*
	for(var i = 0; i < 360; i++)
	{
		r+=15;
		CreateParticle("Flash", Cos(i, currentSize), Sin(i, currentSize), 0, 0, 1, border, 2);
	}
	r+=10;*/
	r+=3;
	var border = {
		Size = currentSize*2,
		BlitMode = GFX_BLIT_Additive,
		R = 255,
		G = 255,
		B = 255,
		Attach=ATTACH_Back,
		Rotation = r,
	};
	CreateParticle("CurlyFries", 0, 0, 0, 0, 1, border, 2);
}

func FxGrowStop(object target, proplist effect, int reason, bool temporary)
{
	CreateParticle("Flash", 0, 0, 0, 0, 0, CenterParticle(), 5);
	AddEffect("SlowEffect", this, 1, 1, this);
}


func FxRemoveStop(object target, proplist effect, int reason, bool temporary)
{
	ClearParticles();
	AddEffect("Shrink", this, 1, 1, this);
}

func FxShrinkStart()
{
	var growcenter =
	{
		Prototype = CenterParticle(),
		Size = currentSize*2,
		Alpha = PV_Linear(255,0)
	};
	
	CreateParticle("Flash", 0, 0, 0, 0, 1, growcenter, 1);
	DrawBorder();
}

func FxShrinkTimer(object target, proplist effect, int time)
{
	if(currentSize <= 0)
	{
		RemoveObject();
		return -1;
	}

	var growcenter =
	{
		Prototype = CenterParticle(),
		Size = currentSize*2,
		Alpha = PV_Linear(255,0)
	};
	
	CreateParticle("Flash", 0, 0, 0, 0, 1, growcenter, 1);
	
	DrawBorder();
	
	if(currentSize > 0)
		currentSize-=1;
}

func FxSlowEffectTimer()
{
	DrawBorder();
}

func FxSlowTimer(object target, proplist effect, int time)
{
	for(var o in FindObjects(Find_Distance(currentSize)))
	{
		if(o->GetOwner() == GetOwner() && o->GetID() == TimeProjectile)
			continue;
		
		if(!GetEffect("SlowField", o))
			AddEffect("SlowField", o, 1, 1, this);
	}
}

func FxSlowFieldStart(object target, proplist effect, int temporary)
{
	if(temporary)
		return;
		
	effect.speed = Distance(0, 0, target->GetXDir(), target->GetYDir());
}

func FxSlowFieldTimer(object target, proplist effect, int time)
{
	if(Distance(target->GetX(), target->GetY(), GetX(), GetY()) > currentSize)
	{
		return -1;
	}
	
	var xdir = target->GetXDir();
	var ydir = target->GetYDir();
	
	var speed = Distance(0, 0, xdir, ydir)*10;
	var angle = Angle(0, 0, xdir, ydir, 100);
	
	if(speed > 5)
		target->SetVelocity(angle, speed - speed/7, 100, 100);
	
	if(target->GetProcedure() != "FLOAT" && target->GetYDir() > 0)
	{
		target->SetYDir(target->GetYDir(100) - 10, 100);
	}
}

func FxSlowFieldStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return -1;

	if(target->GetProcedure() == "FLOAT")
	{
		var speed = target.Speed;
		
		if(!speed)
			speed = effect.speed;
		
		var angle = Angle(0, 0, target->GetXDir(), target->GetYDir());
		target->SetVelocity(angle, speed);
	}
}