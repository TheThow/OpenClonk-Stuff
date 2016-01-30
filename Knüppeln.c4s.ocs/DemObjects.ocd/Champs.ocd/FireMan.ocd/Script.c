/**
	FireMan
	

	@author 
*/


func LaunchSpecial1(int x, int y)
{
	Clonk->LaunchSpell(FireProjectile, x, y, 0, 0);
}

func LaunchSpecial2(int x, int y)
{
	var y_off = 20 - FireNado.size_y;
	Clonk->LaunchSpell(FireNado, x, y, 0, y_off);
}

func JumpEffect(from, to)
{
	Sound("Fire::Inflame", false, 30);

	for(var i = from; i < to; i+=5)
	{
		var r = 10;
		var x = Cos(i, r);
		var y = Sin(i, r);
		
		var trailparticles =
		{
			Prototype = Particles_FireTrail(),
			Size = PV_Linear(10,0),
			Rotation = PV_Linear(360,0)
		};
	
		CreateParticle("Fire", x, y, x, y, 10, trailparticles);
	}
}

func BlockEffect(range)
{
	for(var i = 0; i < 360; i+=10)
	{
		var r = range;
		var x = Cos(i, r);
		var y = Sin(i, r);
		
		var angle = Angle(0,0,x,y) + 90;
		
		var trailparticles =
		{
			Prototype = Particles_FireTrail(),
			Size = PV_Linear(10,0),
			Rotation = PV_Linear(360,0),
		};
	
		CreateParticle("Fire", x, y, 0, 0, 10, trailparticles);
	}
}