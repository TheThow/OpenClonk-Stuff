local ManaCost = 10;
local Speed = 80;
local SpellRange = 300;
local Radius = 150;

func Launch(object clonk, int x, int y)
{
	var particles = 
	{
		Prototype = Particles_Colored(Particles_Glimmer(), GetPlayerColor(clonk->GetOwner())),
	};
	var particle_count = 20;
	while (particle_count--)
	{
		var r_x = RandomX(-Radius, Radius);
		var r_y = RandomX(-Radius, Radius);
		CreateParticle("SphereSpark", r_x, r_y, -r_x, -r_y, PV_Random(5, 30), particles, 5);
	}
	
	var bombs = FindObjects(Find_Distance(Radius), Find_ID(StickyBomb), Find_Owner(GetOwner()));
	if (GetLength(bombs) > 0)
	{
		var modified_speed = Max(Speed - 10 * GetLength(bombs), Speed / 2);
		for (var bomb in bombs)
		{
			bomb->HopTo(GetX(), GetY(), modified_speed);
		}
	}
	RemoveObject();
}

