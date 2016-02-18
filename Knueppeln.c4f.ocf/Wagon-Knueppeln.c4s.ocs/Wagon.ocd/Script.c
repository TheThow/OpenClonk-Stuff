/**
	Wagon
	

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local Health;

local y;

func Initialize()
{
	//SetShape(-30, -24, 60, 48);
	AddEffect("Move", this, 20, 1, this);
	Health = 50;
	SetAction("Travel");
	SetXDir(2);
	y = GetY();
}

//func CanBeHit() { return true; }

func FxMoveTimer()
{
	if(GetX() < -30 || GetX() > LandscapeWidth() + 30)
	{
		RemoveObject();
		return;
	}
	
	//SetXDir(2);
	//SetYDir(0);
}

func FxMoveDamage(object target, proplist effect, int damage, int cause)
{
	if(damage > 0)
	{
		Health = Health - damage;
		if(Health <= 0)
		{
			Die();
		}
	}
}

func Die()
{
	RemoveObject();
}


func SetSteel()
{
	SetGraphics("Steel");
	Health = 999999;
}

local ActMap = {

	Travel = {
		Prototype = Action,
		Name = "Travel",
		Procedure = DFA_FLOAT,
		NextAction = "Travel",
		Length = 1,
		Delay = 1,
		FacetBase = 1,
		Speed = 100,
	},
};