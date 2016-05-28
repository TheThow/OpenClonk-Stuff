local ManaCost = 15;
local Speed = 30;
local MaxTime = 150; 

func CallbackRemove()
{
	RemoveObject();
}

func ArrowIsReflectable()
{
	return true;
}

func Launch(object clonk, int x, int y)
{
	var boompack = clonk->CreateObjectAbove(Boomattack);
	var angle = Angle(0, 0, x, y);
	boompack->SetR(angle);
	boompack->SetController(clonk->GetOwner());
	boompack->SetCon(60);
	boompack->SetObjDrawTransform(600, 0, 0, 0, 600);
	boompack->SetVelocity(angle, Speed);
	boompack.speed = Speed;
	boompack->AddEffect("HitCheck", boompack, 1, 2, nil, nil, clonk);
	ScheduleCall(boompack, "Hit", MaxTime, 0);
	var rider = boompack->CreateContents(Clonk);
	rider.Death = this.CallbackRemove;
	rider->SetObjectBlitMode(GFX_BLIT_Mod2);
	rider->SetClrModulation(clonk->GetColor());
	boompack->OnMount(rider);
	boompack->Sound("Clonk::Skin::Adventurer::Shock*", {pitch = 75});
	RemoveObject();
}

