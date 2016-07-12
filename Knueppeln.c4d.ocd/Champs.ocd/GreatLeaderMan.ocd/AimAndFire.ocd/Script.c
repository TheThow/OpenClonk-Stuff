local ManaCost = 10;
local Speed = 80;
local SpellRange = 300;
local Radius = 150;

local ArrowRemoverEffect = new Effect
{
	Timer = func(int time)
	{
		if (this.Target->Contained()) return;
		if (!this.Target->GetSpeed()) return this.Target->RemoveObject();
	}
};

func OnArrowHit()
{
	RemoveObject();
}

func ArrowIsReflectable()
{
	return true;
}

func Launch(object clonk, int x, int y)
{
	var particles = 
	{
		Size = PV_Linear(30, 0),
		Alpha = PV_KeyFrames(0, 0, 0, 100, 255, 1000, 100),
		Rotation = PV_Step(10)
	};
	particles = Particles_Colored(particles, clonk->GetColor());
	CreateParticle("Selector", 0, 0, 0, 0, 30, particles, 1);
	
	for (var minion in FindObjects(Find_ID(Clonk), Find_Owner(clonk->GetOwner()), Find_Property("IsMinion")))
	{
		if (!minion->IsWalking()) continue;
		var bow = minion->Contents(0);
		if (!bow) continue;
		if (!bow->Contents())
		{
			var arrow = bow->CreateContents(Arrow);
			AddEffect("Traveling", arrow, 1, 1, nil, GetID());
			arrow->SetStackCount(1);
			arrow->SetObjDrawTransform(500, 0, 0, 0, 500);
			arrow.Collectible = false;
			arrow.Hit = this.OnArrowHit;
			arrow->CreateEffect(ArrowRemoverEffect, 1, 5);
			arrow.IsReflectable = this.ArrowIsReflectable;
			arrow.is_minions_arrow = true;
		}
		bow->ControlUseStart(minion, GetX() - minion->GetX(), GetY() - minion->GetY());
		bow->ControlUseStop(minion);
	}
	RemoveObject();
}

func FxTravelingStart(target, fx)
{
	fx.ox = target->GetX();
	fx.oy = target->GetY();
	fx.prt =
	{
		Size = PV_Linear(4,0),
		BlitMode = GFX_BLIT_Additive,
		R = 50,
		G = 50,
		B = 50,
	};
}

func FxTravelingTimer(target, fx)
{
	if(target->Contained())
		return;
		
	//CreateParticle("Flash", 0, 0, 0, 0, 10, trailparticles2, 2);
	DrawParticleLine("Flash", target->GetX(), target->GetY(), fx.ox, fx.oy, 1, 0, 0, 10, fx.prt);
	fx.ox=target->GetX();
	fx.oy=target->GetY();
}

