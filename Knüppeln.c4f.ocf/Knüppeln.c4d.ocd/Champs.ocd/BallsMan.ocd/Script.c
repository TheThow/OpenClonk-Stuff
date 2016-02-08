/**
	FireMan
	

	@author 
*/

#include Man

local Description = "$Description$";
local Name = "$Name$";

local Special1Spell = BallAttackOrder;
local Special2Spell = ElectroOrb;
local Special3Spell = ThunderStrike;

func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		var param = {
			ball = clonk.Ball
		};
		clonk->LaunchSpell(Special1Spell, x, y, 0, 0, param);
		return 1;
	}
	return 0;
}

func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		clonk->LaunchSpell(Special2Spell, x, y, 0, 0);
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
			Size = Special3Spell.SpellRange*2,
			BlitMode = GFX_BLIT_Additive,
			Rotation = PV_Step(10, 0, 1),
			Attach = ATTACH_Back | ATTACH_MoveRelative
			
		};
		clonk->ShowSpellRange(clonk, Special3Spell, props);
	}
	
	if(released && !mouseclick)
	{
		clonk->CancelShowSpellRange();
	}

	if(!released && mouseclick && abletocast)
	{
		if (Sqrt(x**2 + y**2) > Special3Spell.SpellRange)
		{
			var a = Angle(0,0, x, y, 10);
			var newx = Sin(a, Special3Spell.SpellRange, 10);
			var newy = -Cos(a, Special3Spell.SpellRange, 10);
			clonk->LaunchSpell(Special3Spell, newx, newy, newx, newy);
			
			return 1;
		}
		
		clonk->LaunchSpell(Special3Spell, x, y, x, y);
		
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

func InitChamp(clonk)
{
	var ball = clonk->CreateObject(BallsManBall, 0, -15, clonk->GetOwner());
	clonk.Ball = ball;
	ball->SetMaster(clonk);
}

