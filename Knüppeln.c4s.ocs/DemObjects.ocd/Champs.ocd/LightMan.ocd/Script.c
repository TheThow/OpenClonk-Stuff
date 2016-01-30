/**
	LightMan
	

	@author 
*/


func LaunchSpecial1(object clonk, int x, int y)
{
	clonk->LaunchSpell(LightRay, x, y, 0, 0);
}

func LaunchSpecial2(object clonk, int x, int y)
{
	clonk->LaunchSpell(LightStar, x, y, 0, 0);
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

	Sound("electric_shot", false, 30);

	for(var i = from; i < to; i+=5)
	{
		var r = 10;
		var x = clonk->GetX() + Cos(i, r);
		var y = clonk->GetY() + Sin(i, r);
		
		var angle = Angle(0,0,x,y);
		
		var trailparticles =
		{
			Size = PV_Linear(15,0),
			Rotation = angle
		};
	
		CreateParticle("StarSpark", x, y, Cos(i, r), Sin(i, r), 10, trailparticles);
	}
}

func BlockEffect(object clonk, range)
{
	var particles =
	{
		Prototype = Particles_Flash(),
		Size = range,
	};
	CreateParticle("Flash", 0, 0, 0, 0, 5, particles);
}