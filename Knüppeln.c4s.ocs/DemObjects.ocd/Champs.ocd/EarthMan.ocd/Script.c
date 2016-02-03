/**
	FireMan
	

	@author 
*/

#include Man

local Description = "$Description$";
local Name = "$Name$";

func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		clonk->LaunchSpell(EarthProjectile, x, y, 0, 0);
		return 1;
	}
	return 0;
}

func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
/*
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		var angle = Angle(0, 0, x, y);
		var dst = Distance(0, 0, x, y);
		
		if(dst > EarthWall.MaxDst)
		{
			x = Sin(angle, EarthWall.MaxDst);
			y = -Cos(angle, EarthWall.MaxDst);
		}
		
		clonk->LaunchSpell(EarthWall, x, y, x, y);
		return 1;
	}
	return 0;
	*/
	if(!released && !mouseclick)
	{
		var props =
		{
			R = 50,
			G = 255,
			B = 50,
			Alpha = 40,
			Size = EarthWall.SpellRange*2,
			BlitMode = GFX_BLIT_Additive,
			Rotation = PV_Step(10, 0, 1),
			Attach = ATTACH_Back | ATTACH_MoveRelative
			
		};
		clonk->ShowSpellRange(clonk, EarthWall, props);
	}
	
	if(released && !mouseclick)
	{
		clonk->CancelShowSpellRange();
		
	}

	if(!released && mouseclick && abletocast)
	{
		if (Sqrt(x**2 + y**2) > EarthWall.SpellRange)
		{
			var a = Angle(0,0, x, y, 10);
			var newx = Sin(a, EarthWall.SpellRange, 10);
			var newy = -Cos(a, EarthWall.SpellRange, 10);
			clonk->LaunchSpell(EarthWall, newx, newy, newx, newy);
			
			return 1;
		}
		
		clonk->LaunchSpell(EarthWall, x, y, x, y);
		
		return 1;
	}
	
	return 0;
}

func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	return 0;
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
			Size = PV_Linear(8,0),
			BlitMode = GFX_BLIT_Additive,
			R = 50,
			G = 255,
			B = 50,
		};
		if(i%2 == 0)
			CreateParticle("Flash", x, y, 0, 0, 10, trailparticles, 5);
		
		var smoke =
		{
			Alpha = 255,
			Size = PV_Linear(7,0),
			DampingX = 900, DampingY = 900,
			R = 150, G = 120, B = 100,
			Phase = PV_Random(0, 15)
		};
		CreateParticle("Smoke", x, y, PV_Random(-2,2), PV_Random(-2,2), 10, smoke, 2);
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
			Size = PV_Linear(8,0),
			BlitMode = GFX_BLIT_Additive,
			R = 50,
			G = 255,
			B = 50,
		};
		if(i%10==0 || !Random(4))
			CreateParticle("Flash", x, y, 0, 0, 10, trailparticles, 5);
		
		var smoke =
		{
			Alpha = 255,
			Size = PV_Linear(7,0),
			DampingX = 900, DampingY = 900,
			R = 150, G = 120, B = 100,
			Phase = PV_Random(0, 15)
		};
		CreateParticle("Smoke", x, y, PV_Random(-2,2), PV_Random(-2,2), 10, smoke, 4);
	}
}

func FxEarthHitTimer(object target, proplist effect, int time)
{
	var lightning =
	{
		Prototype = Particles_ElectroSpark2(),
		Size = PV_Linear(PV_Random(2,5),0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Random(0,360),
		R = 50,
		G = 255,
		B = 50,
		Attach = ATTACH_Back | ATTACH_MoveRelative,
	};
	
	target->CreateParticle("Lightning", RandomX(-5, 5), RandomX(-10, 10), 0, 0, 10, lightning, 2);
	
	if(time > 40)
		return -1;
}

global func AddEarthHitEffect(object target)
{
	AddEffect("EarthHit", target, 20, 1, nil, EarthMan);
}