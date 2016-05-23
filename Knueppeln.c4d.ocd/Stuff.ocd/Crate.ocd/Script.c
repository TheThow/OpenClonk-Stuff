/*
	Crate
	Author: Ringwaul

	Used for deliveries.
*/

local itemID;

func Initialize()
{
	itemID = GetRandomItem();
}

protected func Construction()
{
	PlayAnimation("Open", 1, Anim_Linear(0, 0, 1, 20, ANIM_Hold));
	SetProperty("MeshTransformation",Trans_Rotate(RandomX(20,80),0,1,0));
	
	AddEffect("CheckDmg", this, 1, 2, this);
	
	return _inherited(...);
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
	CreateObject(itemID, 0, -6, -1);
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