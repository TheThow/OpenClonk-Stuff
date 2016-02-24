local ManaCost = 10;
local Speed = 60;
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
	
	for (var bomb in FindObjects(Find_Distance(Radius), Find_ID(StickyBomb), Find_Owner(GetOwner())))
	{
		bomb->HopTo(GetX(), GetY(), Speed);
	}
	RemoveObject();
}

