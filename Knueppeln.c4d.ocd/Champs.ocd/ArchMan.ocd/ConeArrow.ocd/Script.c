local pR = 100;
local pG = 100;
local pB = 250;
local Speed = 100;
//local SpellDamage = 8;
local ManaCost = 35;
local Range = 200;

func Launch(object clonk, int x, int y)
{
	for (var i = 0; i < 7; i++)
	{
		var arrow = CreateObject(ConalArrow);
		var anglediff = (i - 3) * 50;
		arrow->Launch(clonk, x, y, anglediff);
	}
	RemoveObject();
}
