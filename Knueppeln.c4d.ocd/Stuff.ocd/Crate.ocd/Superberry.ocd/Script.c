/*-- Sproutberry --*/

local probability = 30;

protected func Hit()
{
	Sound("Hits::SoftHit1");
}

public func Construction()
{
	this.MeshTransformation = Trans_Scale(1500, 1500, 1500);
}

/* Eating */

protected func ControlUse(object clonk, int iX, int iY)
{
	Sound("Clonk::Action::Munch?");
	//clonk->DoMagicEnergy(50, 1);
	
	var props = { 
		DampingX = 950, DampingY = 950,
		Alpha = PV_Linear(255, 0),
		Size = PV_Linear(10, 0),
		R = 255,
		G = 255,
		B = 0,
		BlitMode = GFX_BLIT_Additive,
	};
	
	clonk->CreateParticle("Flash", PV_Random(-5, 5), PV_Random(-10,10), PV_Random(-10, 10), PV_Random(-10,10), 40, props, 20);
	
	var fx = AddEffect("Superberry", clonk, 1, 1, nil, GetID());
	fx.dummy = CreateObject(Dummy, 0, 0, clonk->GetOwner());
	fx.dummy->SetAction("HangOnto", clonk);
	var props =
	{
		R = 255,
		G = 255,
		B = 50,
		Alpha = 40,
		Size = 60,
		BlitMode = GFX_BLIT_Additive,
		Rotation = PV_Step(10, 0, 1),
		Attach = ATTACH_Back | ATTACH_MoveRelative
		
	};
	fx.dummy->CreateParticle("StarSpark", 0, 0, 0, 0, 0, props, 4);
	fx.dummy.Visibility = VIS_All;
	fx.dummy.Plane = 600;
	
	clonk->Sound("superberry", false, 50, nil, 0);
	
	RemoveObject();
	return true;
}

func FxSuperberryTimer(target, fx, timer)
{
	if (timer > 40*5)
	{
		return -1;
	}
	
	if(target->GetAction() == "Tumble")
		target->SetAction("Jump");
		
	var rgba = SplitRGBaValue(HSL(Random(255), 255, 128));
	
	var props = {
		R = rgba[0],
		G = rgba[1],
		B = rgba[2],
		Size = PV_Linear(5, 0),
		Alpha = PV_Linear(255,0),
		OnCollision = PC_Bounce(),
		ForceY = PV_Gravity(200),
		BlitMode = GFX_BLIT_Additive
	};
	
	target->CreateParticle("StarSpark", PV_Random(-5, 5), PV_Random(-10, 10), PV_Random(-4,4), PV_Random(-20, -5), 20, props, 1);
	
	target->SetClrModulation(HSL(timer*20, 255, 128));
}

func FxSuperberryDamage()
{
	return 0;
}

func FxSuperberryStop(object target, proplist fx, int reason, bool temporary)
{
	if(temporary)
		return;
	
	if (fx.dummy)
		fx.dummy->RemoveObject();
	
	target->SetClrModulation(RGB(255,255,255));
}

public func NutritionalValue() { return 5; }

local Name = "$Name$";
local Description = "$Description$";
local Collectible = 1;
