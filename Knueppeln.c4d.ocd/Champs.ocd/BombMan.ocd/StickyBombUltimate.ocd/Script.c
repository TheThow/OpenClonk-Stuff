local ManaCost = 50;
local ChargeDuration = 40;
local BombCount = 12;
local Speed = 60;
local Target;

func Launch(object clonk, int x, int y)
{
	var params = {
		angle = Angle(0,0,x,y),
		cl = clonk
	};
	clonk->Charge(this, "ChargeStop", ChargeDuration, params);
	Target = clonk;
	SetController(clonk->GetController());
}

func ChargeInterrupted()
{
	RemoveObject();
}

func ChargeStop(proplist params)
{
	Sound("UI::Cash*", false, 100, nil, nil, nil, 200);
	var angle = 360 / BombCount;
	for (var i = 0; i < BombCount; ++i)
	{
		var a = angle * i;
		var bomb = CreateObject(StickyBomb, Sin(a, 20), -Cos(a, 20), GetController());
		if (bomb->Stuck())
		{
			bomb->RemoveObject();
			continue;
		}
		bomb->Launch(params.cl);
		bomb->SetSpeed(Sin(a, Speed), -Cos(a, Speed)); 
	}
	RemoveObject();
}