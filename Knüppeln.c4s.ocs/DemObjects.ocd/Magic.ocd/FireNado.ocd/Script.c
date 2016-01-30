/**
	FireNado
	

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local ManaCost = 40;

local size_x = 25;
local size_y = 30;
local SpellDamage = 30;
local a = 0;

local Speed = 30;
local Durr = 76;
local Charge_durr = 20;

func Initialize()
{
	SetAction("Travel");
	SetClrModulation(RGBa(0,0,0,0));
}

func Launch(object clonk, int x, int y)
{
	var params = {
		angle = Angle(0,0,x,y)
	};
	clonk->Charge(this, "ChargeStop", Charge_durr, params);
}

func ChargeStop(proplist params)
{
	var angle = params.angle;
	SetVelocity(angle, Speed);
	
	AddEffect("FireNado", this, 20, 1 ,this, GetID());
	AddEffect("Remove", this, 20, Durr, this, GetID());
	
	Sound("Fire::Fireball", false, 100);
	Sound("Fire::FuseLoop", false, 20, nil, 1);
}

func FxChargeStop(object target, proplist effect, int reason, bool temporary)
{
	effect.clonk->SetAction("Jump");
	var angle = effect.new_angle;
	SetVelocity(angle, Speed);
	
	AddEffect("FireNado", this, 20, 1 ,this, GetID());
	AddEffect("Remove", this, 20, Durr, this, GetID());
	
	Sound("Fire::Fireball", false, 100);
	Sound("Fire::FuseLoop", false, 20, nil, 1);
}

func FxFireNadoTimer(object target, proplist effect, int time)
{
	for(var o in FindObjects(Find_InRect(-size_x/2, -size_y/2, size_x, size_y), Find_Or(Find_ID(Clonk), Find_Func("IsReflectable"))))
	{
		if (o->GetID() == Clonk)
		{
			if(!GetEffect("NadoCD", o) && o->GetOwner() != GetOwner())
			{
				o->Fling(0, -5);
				o->DoEnergy(SpellDamage);
				AddEffect("NadoCD", o, 20, 10);
			}
		}
		
		if( o->~IsReflectable())
		{
			var speed = o->GetID().Speed;
			var angle = RandomX(-20,20);
			
			o->SetVelocity(angle, speed);
		}
	}
	
	var detail = size_y*2;
	
	for(var i = 0; i > -detail; i--)
	{
		var firetrailparticles =
		{
			Prototype = Particles_FireTrail(),
			Size = PV_Linear(5,0)
		};
	
		var r = (360 * 3) * i / detail;
		CreateParticle("Fire", Cos(r + a, size_x * i / detail / 2), size_y/2 + size_y * i / detail, 0, 0, 5, firetrailparticles);
		
		if(time%3)
			a++;
	}
	
	return 0;
}


func FxRemoveStop()
{
	RemoveObject();
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
		StartCall = "Traveling",
		Speed=1000
	},
};
