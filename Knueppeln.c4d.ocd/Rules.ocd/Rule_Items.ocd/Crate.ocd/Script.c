/*
	Crate
	Author: Ringwaul

	Used for deliveries.
*/

local itemID;

local RemoveFx = new Effect {

	Timer = func()
	{
		if(!Target)
			return -1;
	
		this.timer ++;
		Target->SetClrModulation(RGBa(255, 255, 255, 255 - this.timer));
		
		if(this.timer == 255)
			Target->RemoveObject();
	}

};

func Initialize()
{
	itemID = GetRandomItem();
}

func CanBeSucked() { return true; }

protected func Construction()
{
	PlayAnimation("Open", 1, Anim_Linear(0, 0, 1, 20, ANIM_Hold));
	SetProperty("MeshTransformation",Trans_Rotate(RandomX(20,80),0,1,0));
	
	AddEffect("CheckDmg", this, 1, 2, this);
	AddEffect("FadeOut", this, 1, 600, this);
	
	return _inherited(...);
}

func FxFadeOutTimer()
{
	CreateEffect(RemoveFx, 1, 1);
}

func FxCheckDmgTimer()
{
	if(GBackLiquid())
	{
		Pop();
	}

	return 0;
}

func FxCheckDmgDamage()
{
	Pop();
}

func Pop()
{
	CreateObject(itemID, 0, -itemID->GetDefHeight()/2 + GetDefHeight()/2 - 1, -1);
	HitEffect();
	Sound("Hits::Materials::Wood::WoodHit?", false, 50);
	RemoveObject();
}

func HitEffect()
{
	CreateParticle("WoodChip", 0, 0, PV_Random(-30, 30), PV_Random(-30, 10), 100, Particles_WoodChip(), 50);
}

func Hit()
{
	Sound("Hits::Materials::Wood::DullWoodHit?", false, 50);
}

func SetItemID(id ID)
{
	itemID = ID;
}

func CanBeHit()
{
	return true;
}

func IsProjectileTarget(object o) { return true; }

local Name = "$Name$";
local Description = "$Description$";