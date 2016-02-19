/**
	GravityThing
	

	@author 
*/

local partner;

func Initialize()
{
	AddEffect("Particles", this, 1, 1 ,this);
}

func CreatePartner()
{
	partner = CreateObject(GetID(), 0, -30);
}

func FxParticlesTimer()
{
	if(!partner)
	{
		return;
	}
	
	var props = {
		Size = PV_Linear(PV_Random(3, 5),0),
		BlitMode = GFX_BLIT_Additive,
		R = 50,
		G = 255,
		B = 50,
	};
	
	
	var dist = Distance(GetX(), GetY(), partner->GetX(), partner->GetY());
	var angle = Angle(GetX(), GetY(), partner->GetX(), partner->GetY());
	
	for(var i = 0; i < dist/10; i++)
	{
		var r = Random(dist);
		var x = Cos(angle - 90, r) + RandomX(-3,3);
		var y = Sin(angle - 90, r) + RandomX(-3, 3);
		CreateParticle("Flash", x, y, 0, 0, 10, props, 1);
	}
}

func FxParticlesStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;

	if(partner)
		partner->RemoveObject();
}