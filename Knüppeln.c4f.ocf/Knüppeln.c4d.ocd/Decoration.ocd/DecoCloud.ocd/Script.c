/**
	Testi
	Insert description here

	@author 
*/

local Plane = 901;
local maxY;
local minY;

func Initialize()
{
	//DoCon(Random(75));

	SetAction("Float");
	SetPhase(RandomX(1,16));
	AddEffect("MoveCloud", this, 100, 5, this);
	minY = 0;
	maxY = LandscapeHeight();
}

func SetMaxY(int y)
{
	maxY = y;
}

func SetMinY(int y)
{
	minY = y;
}

private func FxMoveCloudTimer()
{
	// Get wind speed from various locations of the cloud.
	var con = GetCon();
	var wdt = GetDefWidth() * con / 100;
	var hgt = GetDefHeight() * con / 100;
	var xoff = wdt * 10 / 25;
	var yoff = hgt * 10 / 35;
	var wind = (GetWind() + GetWind(xoff, yoff) + GetWind(xoff, -yoff) + GetWind(-xoff, -yoff) + GetWind(-xoff, yoff) + GetWind(nil, nil, true)) / 6;
	/*
	// Move according to wind.
	if (Abs(wind) < 7)
		SetXDir(0);
	else
		SetXDir(wind * 10, 1000);
	*/
	// Loop clouds around the map.
	
	var y = RandomX(minY, maxY);
	
	if (GetX() >= LandscapeWidth() + wdt/2 - 60) 
	{
		SetPosition(62 - wdt/2, y);
		SetPhase(RandomX(1,16));
	}
	else if (GetX() <= 60 - wdt/2) 
	{
		SetPosition(LandscapeWidth() + wdt/2 - 62, y);
		SetPhase(RandomX(1,16));
	}
		
	return;
}

local ActMap = {

	Float = {
		Prototype = Action,
		Name = "Float",
		Procedure = DFA_FLOAT,
		NextAction = "Float",
		Length = 16,
		FacetBase=1,
		X=0,
		Y=0,
		Delay = 0,
		Wdt = 512,
		Hgt = 350,
		Speed = 100,
	}
};