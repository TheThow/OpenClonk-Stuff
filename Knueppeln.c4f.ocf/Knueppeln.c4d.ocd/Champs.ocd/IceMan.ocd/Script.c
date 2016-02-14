/**
	IceMan
	

	@author 
*/

#include Man

local Name = "$Name$";
local Description = "$Description$";

local Special1Spell = IceProjectile;
local Special2Spell = IceShard;
local Special3Spell = IceShardUltimate;

func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast)
{
	if(!released && !mouseclick && abletocast)
		clonk->LaunchSpell(IceProjectile, x, y, 0, 0);
}

func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast)
{
	if (released || mouseclick) return;
	
	var existing = FindObject(Find_ID(IceShard), Find_Owner(clonk->GetOwner()), Find_Category(C4D_StaticBack), Find_Func("GetProperty", "is_selected"));
	if (existing)
	{
		existing->FireNow();
		var other = FindObjects(Find_ID(IceShard), Find_Owner(clonk->GetOwner()), Find_Category(C4D_StaticBack), Sort_Func("GetProperty", "number"));
		if (GetLength(other))
		{
			other[0].is_selected = true;
		}
		return;
	}
	
	if(abletocast)
	{
		clonk->LaunchSpell(IceShard, x, y, 0, 0);
	}
}

func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast)
{
	if (released || mouseclick) return;
	
	var existing = FindObject(Find_ID(IceShardUltimate), Find_Owner(clonk->GetOwner()), Find_Category(C4D_StaticBack), Find_Func("GetProperty", "is_selected"));
	if (existing)
	{
		existing->FireNow();
		var other = FindObjects(Find_ID(IceShardUltimate), Find_Owner(clonk->GetOwner()), Find_Category(C4D_StaticBack), Sort_Func("GetProperty", "number"));
		if (GetLength(other))
		{
			other[0].is_selected = true;
		}
		return;
	}
	
	if(abletocast)
	{
		clonk->LaunchSpell(IceShardUltimate, x, y, 0, 0);
	}
}

func JumpEffect(object clonk, dir)
{
	var from, to;

	if (dir == "Up")
	{
		from = 50; 
		to = 130;
	}
	if (dir == "Left")
	{
		from = -30; 
		to = 50;
	}
	if (dir == "Right")
	{
		from = 130; 
		to = 210;
	}
	if (dir == "Down")
	{
		from = 230; 
		to = 310;
	}

	Sound("Fire::Inflame", false, 30, nil, nil, nil, 200);
	
	var trailparticles =
	{
		R = 255,
		G = 255,
		B = 255,
		Size = PV_Linear(PV_Random(4, 6), 0),
		OnCollision = PC_Die(),
		CollisionVertex = 0,
		ForceX = PV_Linear(0, PV_Random(-20, 20, 10)),
		ForceY = PV_Linear(0, PV_Random(-20, 20, 10)),
		DampingX = 900, DampingY = 900,
		BlitMode = GFX_BLIT_Additive
	};
	
	for(var i = from; i < to; i+=5)
	{
		var r = 10;
		var x = Cos(i, r);
		var y = Sin(i, r);
		clonk->CreateParticle("StarFlash", x, y, Cos(i, r), Sin(i, r), 60, trailparticles, 10);
	}
}

func BlockEffect(object clonk, int radius)
{
	var border = 
	{
		R = 255,
		G = 255,
		B = 255,
		Size = PV_Linear(PV_Random(4, 6), 0),
		OnCollision = PC_Die(),
		CollisionVertex = 0,
		ForceX = PV_Linear(0, PV_Random(-10, 10, 10)),
		ForceY = PV_Linear(0, PV_Random(-10, 10, 10)),
		DampingX = 900, DampingY = 900,
		BlitMode = GFX_BLIT_Additive
	};
	
	for (var angle = 0; angle < 360; angle += 20)
	{
		clonk->CreateParticle("StarFlash", Sin(angle + RandomX(-5, 5), radius), -Cos(angle + RandomX(-5, 5), radius), 0, 0, PV_Random(20, 30), border, 16);
	}
}

func FxFireHitTimer(object target, proplist effect, int time)
{
	var chaoticspark =
	{
		Size = PV_Linear(1, 0),
		ForceX = PV_KeyFrames(10, 0, PV_Random(-6, 6), 333, PV_Random(-6, -6), 666, PV_Random(6, 6), 1000, PV_Random(-6, 6)),
		ForceY = PV_KeyFrames(10, 0, PV_Random(-8, 5), 333, PV_Random(-8, 5), 666, PV_Random(-10, 10), 1000, PV_Random(-10, 15)),
		Stretch = PV_Speed(1000, 500),
		Rotation = PV_Direction(),
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		R = 255,
		G = PV_Linear(255,200),
		B = PV_Random(0, 100),
		DampingX=950,
		DampingY=950,
		Alpha = PV_Random(100,180),
		BlitMode = GFX_BLIT_Additive
	};
	var sharpflame =
	{
		Size = 4,
		R = 255,
		G = PV_KeyFrames(0, 0, 255, 666, 0, 627, 0),
		B = 0,
		Rotation = PV_Random(0, 360),
		Phase = PV_Random(0, 5),
		OnCollision = 0,
		BlitMode = GFX_BLIT_Additive,
		Attach = ATTACH_Front | ATTACH_MoveRelative
	};
	
	var x = target->GetX();
	var y = target->GetY();
	
	CreateParticle("Magic", PV_Random(x + -5, x+ 5), PV_Random(y + -10, y + 10), PV_Random(25, -25), PV_Random(-25, 12), 50, chaoticspark);
	
	CreateParticle("FireSharp", RandomX(x + -5, x+ 5), RandomX(y + -10, y + 10), 0, PV_Random(-3,-10), 10, sharpflame, 2);
	
	if(time > 40)
		return -1;
}

global func AddFireHitEffect(object target)
{
	this->AddEffect("FireHit", target, 20, 1, nil, FireMan);
}