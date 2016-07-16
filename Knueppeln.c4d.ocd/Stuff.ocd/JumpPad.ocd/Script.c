/**
	JumpPad
	

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local strength;

local JumpPadFx = new Effect {

	Timer = func()
	{
		var r = Target->GetR() - 90;
		
		for (obj in Target->FindObjects(Find_OnLine(Cos(r-75, 18), Sin(r-75, 22), Cos(r+75, 18), Sin(r+75,22)), Find_ID(Clonk)))
		{
			if(GetEffect("JumpPadCD", obj))
				continue;
		
			obj->SetAction("Jump");
			//Log("%d", Target->GetR());
			//obj->SetVelocity(Target->GetR(), Target.strength);
			obj->SetYDir(-Target.strength);
			obj->AddEffect("JumpPadCD", obj, 1, 5);
			Target->Sound("boing", false, 50);
		}
	
	}

};

func Initialize()
{
	strength = 100;
	CreateEffect(JumpPadFx, 1, 1);

}