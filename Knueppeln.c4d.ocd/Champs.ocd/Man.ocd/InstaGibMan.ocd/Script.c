/**
	FireMan
	

	@author 
*/

#include Man

local Description = "$Description$";
local Name = "$Name$";

local Special1Cooldown = 20;

local Special1Spell = InstaProjectile;
local Special2Spell = InstaProjectile;
local Special3Spell = InstaProjectile;

func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		clonk->LaunchSpell(Special1Spell, x, y, 0, 0);
		return 1;
	}
	return 0;
}

func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		Sound("UI::Error", false, 50, clonk->GetOwner());
		return 0;
	}
	return 0;
}

func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		Sound("UI::Error", false, 50, clonk->GetOwner());
		return 0;
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
			R = 255,
			G = 50,
			B = 50,
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
			R = 255,
			G = 50,
			B = 50,
		};
	
		CreateParticle("Lightning", x, y, 0, 0, 10, trailparticles);
	}
	
}

func CanCastSpecial2(object clonk)
{
	return false;
}

func CanCastSpecial3(object clonk)
{
	return false;
}