/**
	FireMan
	

	@author 
*/

#include Man

local Description = "$Description$";

func LaunchSpecial1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast)
{
	if(!released && !mouseclick && abletocast)
		clonk->LaunchSpell(FireProjectile, x, y, 0, 0);
}

func LaunchSpecial2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast)
{
	if(!released && !mouseclick && abletocast)
	{
		var y_off = -6;
		clonk->LaunchSpell(FireNado, x, y, 0, y_off);
	}
}

func LaunchSpecial3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast)
{
	if(!released && !mouseclick)
	{
		var props =
		{
			R = 255,
			G = 215,
			B = 0,
			Alpha = 40,
			Size = FireDash.SpellRange*2 + FireDash.SpellRange/50*20,
			BlitMode = GFX_BLIT_Additive,
			Rotation = PV_Step(10, 0, 1),
			Attach = ATTACH_Back | ATTACH_MoveRelative
			
		};
		clonk->ShowSpellRange(clonk, FireDash, props);
	}
	
	if(released && !mouseclick)
	{
		clonk->CancelShowSpellRange();
	}

	if(!released && mouseclick && abletocast)
	{
		var solidcheck = false;
		var cx = clonk->GetX();
		var cy = clonk->GetY();
		
		for(var i = 0; i < 360; i++)
		{
			if(GBackSolid(cx + x + Sin(i, 5), cy + y + Cos(i, 5)))
				solidcheck = true;
		}
	
		if (Sqrt(x**2 + y**2) > FireDash.SpellRange
			|| solidcheck)
		{
			Sound("UI::Error", false, 50, clonk->GetOwner());
			return 1;
		}
		
		clonk->LaunchSpell(FireDash, x, y, x, y);
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

	Sound("Fire::Inflame", false, 30);

	for(var i = from; i < to; i+=5)
	{
		var r = 10;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var trailparticles =
		{
			Prototype = Particles_FireTrail(),
			Size = PV_Linear(10,0),
			Rotation = PV_Linear(360,0)
		};
	
		CreateParticle("Fire", x, y, Cos(i, r), Sin(i, r), 10, trailparticles);
	}
}

func BlockEffect(object clonk, range)
{
	for(var i = 0; i < 360; i+=5)
	{
		var r = range;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var trailparticles =
		{
			Prototype = Particles_FireTrail(),
			Size = PV_Linear(10,0),
			Rotation = PV_Linear(360,0),
		};
	
		CreateParticle("Fire", x, y, 0, 0, 10, trailparticles, 2);
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
	AddEffect("FireHit", target, 20, 1, nil, FireMan);
}