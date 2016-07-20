/**
	DummySpell
	

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local ManaCost = 0;

func Launch(clonk, x, y)
{
	Fireworks(HSL(Random(255), 255, 127) , 0, 0);
	SoundAt("vanish", 0, 0, 50);
	RemoveObject();
}