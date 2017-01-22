/**
	Rule_RandomChamp
	

	@author 
*/



func Initialize()
{
	
}

func Set(int time)
{
	AddEffect("SpawnItems", this, 1, time, this);
}

global func GetRandomItem()
{
	var items = [Superberry, Boompack, MonsterBall, BlackHole, RubberDucky, FakeHole];
	var l = [];
	
	for (item in items)
	{
		for(var i = 0; i < item.probability; i++)
			PushBack(l, item);
	}
	
	return l[Random(GetLength(l))];
}

func FxSpawnItemsTimer()
{
	if(Random(Max(3, 8 - GetPlayerCount())))
		return;

	var pos = GameCall("GetItemCratePos");
	
	if (pos == nil)
		return -1;
	
	var o = CreateObject(ItemCrate, pos[0] - GetX(), pos[1] - GetY(), -1);
	o->SetYDir(-2);
	o->SetRDir(3 * (Random(2) * 2 - 1));
	
	var props = {
		R = 50,
		G = 255,
		B = 255,
		Size = PV_Linear(15, 0),
		Alpha = PV_Linear(255, 0),
		BlitMode = GFX_BLIT_Additive,
	};
	
	for (var i = 0; i < 360; i+=10)
	{
		CreateParticle("Flash", pos[0] + Cos(i, 10) - GetX(), pos[1] + Sin(i, 10) - GetY(), Cos(i, 5), Sin(i, 5), 20, props, 2);
	}
}