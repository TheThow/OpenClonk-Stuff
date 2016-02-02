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
		clonk->LaunchSpell(ElectroProjectile, x, y, 0, 0);
		return 1;
	}
	return 0;
}

func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		clonk->LaunchSpell(ElectroOrb, x, y, 0, 0);
		return 1;
	}
	return 0;
}

func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick)
	{
		var props =
		{
			R = 150,
			G = 215,
			B = 255,
			Alpha = 40,
			Size = ThunderStrike.SpellRange*2 + ThunderStrike.SpellRange/50*20,
			BlitMode = GFX_BLIT_Additive,
			Rotation = PV_Step(10, 0, 1),
			Attach = ATTACH_Back | ATTACH_MoveRelative
			
		};
		clonk->ShowSpellRange(clonk, ThunderStrike, props);
	}
	
	if(released && !mouseclick)
	{
		clonk->CancelShowSpellRange();
		
	}

	if(!released && mouseclick && abletocast)
	{
		if (Sqrt(x**2 + y**2) > ThunderStrike.SpellRange)
		{
			Sound("UI::Error", false, 50, clonk->GetOwner());
			return 0;
		}
		
		clonk->LaunchSpell(ThunderStrike, x, y, x, y);
		
		if(clonk.RangeDummy.range_on == false)
		{
			Log("This shouldn't happen");
			Log("special1: %d special2: %d special3: %d", clonk.special_active[1], clonk.special_active[2], clonk.special_active[3]);
		}
		return 1;
	}
	
	return 0;
}

func JumpEffect(object clonk, dir)
{
	var from;
	var to;

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

	Sound("electro_shot", false, 30);

	for(var i = from; i < to; i+=5)
	{
		var r = 10;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var angle = Angle(0,0,Cos(i, r),Sin(i, r));
		
		var trailparticles =
		{
			Prototype = Particles_ElectroSpark2(),
			Size = PV_Linear(10,0),
			Rotation = angle,
			R = 150,
			G = 215,
			B = 255,
			OnCollision = PC_Bounce(),
		};
	
		CreateParticle("Lightning", x, y, Cos(i, r), Sin(i, r), 10, trailparticles);
	}
}

func BlockEffect(object clonk, range)
{
	for(var i = 0; i < 360; i+=10)
	{
		var r = range;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var angle = Angle(0,0,Cos(i, r),Sin(i, r)) + 90;
		
		var trailparticles =
		{
			Prototype = Particles_ElectroSpark2(),
			Size = PV_Linear(15,0),
			Rotation = angle,
			R = 150,
			G = 215,
			B = 255,
		};
	
		CreateParticle("Lightning", x, y, 0, 0, 10, trailparticles);
	}
	
}

func ShowRange()
{

}

func FxElectroHitTimer(object target, proplist effect, int time)
{
	var lightning =
	{
		Prototype = Particles_ElectroSpark2(),
		Size = PV_Linear(PV_Random(2,5),0),
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Random(0,360),
		R = 175,
		G = 215,
		B = 255,
		Attach = ATTACH_Back | ATTACH_MoveRelative,
	};
	
	target->CreateParticle("Lightning", RandomX(-5, 5), RandomX(-10, 10), 0, 0, 10, lightning, 2);
	
	if(time > 40)
		return -1;
}

global func AddElectroHitEffect(object target)
{
	AddEffect("ElectroHit", target, 20, 1, nil, ElectroMan);
}