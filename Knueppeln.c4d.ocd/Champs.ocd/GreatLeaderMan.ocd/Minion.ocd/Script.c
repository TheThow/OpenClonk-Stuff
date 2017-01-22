local ManaCost = 10;
local Speed = 80;
local SpellRange = 300;
local Radius = 150;

func Launch(object clonk, int x, int y)
{
	var minioncount = clonk->ObjectCount(Find_Distance(20), Find_ID(Clonk), Find_Property("IsMinion"));
	var minion = clonk->CreateObject(Clonk, 0, 0, clonk->GetOwner());
	minion->SetCon(50);
	minion->SetAction("Tumble");
	minion->Fling(0, -4);
	minion->SetXDir(RandomX(-minioncount, minioncount));
	minion->SetObjectBlitMode(GFX_BLIT_Mod2);
	minion->SetClrModulation(clonk->GetColor());
	var bow = minion->CreateContents(Bow);
	bow.animation_set.LoadTime = 15;
	bow.animation_set.LoadTime2 = 5*15/20;
	minion->StartAim(minion->Contents());
	minion->Sound("Clonk::Skin::Alchemist::EvilConfirm*", {pitch = 75});
	minion.Hurt = nil;
	minion.MaxEnergy = 3000;
	minion->DoEnergy(3);
	minion.Death = this.MinionDeath;
	minion.CatchBlow = this.MinionCatchBlow;
	minion.IsMinion = true;
	RemoveEffect("UpdateHUD", minion);
	minion->~RemoveHUD();
	RemoveObject();
}

func MinionDeath()
{
	if ((GetKiller() == GetOwner()) && !Random(3)) Sound("Clonk::Skin::Alchemist::Doubt*", {pitch = 75});
	else Sound("Clonk::Skin::Alchemist::Scream*", {pitch = 75});
	var parts = 
	{
		Prototype = Particles_Material(RGB(255, 0, 0)),
		Size = PV_Linear(1, 0)
	};
	CreateParticle("SphereSpark", 0, 0, PV_Random(-10, 10), PV_Random(-10, -5), PV_Random(10, 30), parts, 60);
	RemoveObject();
}

func MinionCatchBlow()
{
	Sound("Clonk::Skin::Alchemist::Hurt*", {pitch = 75});
}
