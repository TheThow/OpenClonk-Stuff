/**
	LightMan
	

	@author 
*/

#include Man

local Description = "$Description$";

func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast)
{
	if(!released && !mouseclick && abletocast)
		clonk->LaunchSpell(LightStar, x, y, 0, 0);
}

func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast)
{
	if(!released && !mouseclick && abletocast)
		clonk->LaunchSpell(LightRay, x, y, 0, 0);
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
			Size = PV_Linear(15,0),
			Rotation = angle
		};
	
		CreateParticle("StarSpark", x, y, Cos(i, r), Sin(i, r), 10, trailparticles);
	}
}

func BlockEffect(object clonk, range)
{
	var ray = 
	{
		Size = 16,
		Alpha = PV_Linear(255,0),
		BlitMode = GFX_BLIT_Additive,
		G = 0, B = 0
	};
	for (var angle = 0; angle < 360; angle += 10)
	{
		ray.Rotation = angle + 90;
		var from_x = clonk->GetX();
		var from_y = clonk->GetY();
		var off_x = +Sin(angle, range);
		var off_y = -Cos(angle, range);
		var to_x = from_x + off_x;
		var to_y = from_y + off_y;
		CreateParticle("RaySpark", to_x, to_y, 0, 0, 10, ray, 1);
		if (!Random(5))
		{
			ray.Rotation = angle;
			DrawParticleLine("RaySpark", from_x + off_x/4, from_y + off_y/4, from_x + 3 * off_x / 4, from_y + 3 * off_y / 4, 8, 0, 0, 10, ray);	
		}
	}
}