/**
	FireMan
	

	@author 
*/

#include Man

local Description = "$Description$";
local Name = "$Name$";

local Special1Spell = PoisonDart;
local Special2Spell = ShadowWalk;
local Special3Spell = CriticalHit;

local Special2Cooldown = 150;

func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		clonk->LaunchSpell(Special1Spell, x, y, 0, 0);
		if(GetEffect("ShadowWalk", clonk))
			RemoveEffect("ShadowWalk", clonk);
			
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
	if(!released && !mouseclick && abletocast && !cooldown)
	{
		clonk->LaunchSpell(Special3Spell, x, y, 0, 0);
		if(GetEffect("ShadowWalk", clonk))
			RemoveEffect("ShadowWalk", clonk);
			
		return 1;
	}
	
	return 0;
}

func LeftClick(object clonk, int x, int y, bool released, bool abletocast)
{
	if(GetEffect("ShadowWalk", clonk))
		RemoveEffect("ShadowWalk", clonk);
}

func JumpEffect(object clonk, dir)
{
	var lifetime = 10;
	var dummy = CreateObject(Dummy, clonk->GetX(), clonk->GetY(), clonk->GetOwner());
	dummy.Visibility = VIS_Owner;
	AddEffect("Remove", dummy, 1, lifetime, nil, NinjaMan);

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

	Sound("Hits::SoftHit2", false, 30);

	for(var i = from; i < to; i+=5)
	{
		var r = 10;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var trailparticles =
		{
			Size = PV_Linear(5,0),
			R = 150,
			G = 150,
			B = 150,
			OnCollision = PC_Bounce(),
			Attach = ATTACH_Back,
		};
		
		if(GetEffect("ShadowWalk", clonk))
			dummy->CreateParticle("Flash", x - clonk->GetX(), y - clonk->GetY(), Cos(i, r), Sin(i, r), lifetime, trailparticles);
		else
			CreateParticle("Flash", x, y, Cos(i, r), Sin(i, r), lifetime, trailparticles);
	}
}

func BlockEffect(object clonk, range)
{

	for(var i = 0; i < 360; i+=3)
	{
		var r = range;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var props =
		{
			Size = PV_Linear(5,0),
			R = 150,
			G = 150,
			B = 150,
		};
	
		CreateParticle("Flash", x, y, 0, 0, 10, props);
	}
	
}

func FxRemoveStop(target)
{
	target->RemoveObject();
}