/**
	FireMan
	

	@author 
*/


func LaunchSpecial1(object clonk, int x, int y)
{
	clonk->LaunchSpell(FireProjectile, x, y, 0, 0);
}

func LaunchSpecial2(object clonk, int x, int y)
{
	var y_off = -6;
	clonk->LaunchSpell(FireNado, x, y, 0, y_off);
}

func JumpEffect(object clonk, from, to)
{
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
	for(var i = 0; i < 360; i+=10)
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
	
		CreateParticle("Fire", x, y, 0, 0, 10, trailparticles);
	}
}