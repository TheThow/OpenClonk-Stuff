/**
	WormyMcWormFace
	

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local FollowFx = new Effect
{
	Timer = func()
	{
		if (this.np != nil)
		{
			if(this.r)
			{
				Target->RemoveObject();
				return -1;
			}
			Target->SetPosition(this.np[0], this.np[1]);
		}
		else
			this.np = CreateArray(2);
		
		if(this.n)
			this.np = [this.n->GetX(), this.n->GetY()];
		else
			this.r = true;
		
	}

};

local DamageFx = new Effect
{
	Timer = func()
	{
		for(var o in Target->FindObjects(Find_Distance(25), Find_Func("CanBeHit"), Find_Not(Find_ID(Target->GetID()))))
		{
			if (GetEffect("WormyHitCD", o))
				continue;
			
			Sound("Hits::ProjectileHitLiving*", false, 50);
		
			var angle = Angle(Target->GetX(), Target->GetY(), o->GetX(), o->GetY());
			AddEffect("WormyHitCD", o, 1, 15);
			o->Fling(Sin(angle, 8), -Cos(angle, 8) - 2);
			Target->WeaponDamage(o, 40);
		}
	
	}
};

local WormyAppearFx = new Effect
{
	Start = func()
	{
		SoundAt("wormy_dig", Target->GetX(), Target->GetY(), 100);
	},
	
	Timer = func()
	{
		this.timer++;

		CreateParticle("SmokeThick", this.x, PV_Random(this.y, this.y + 10), PV_Random(-30,30), PV_Random(-5,5), PV_Random(40, 60), this.p1, 2);
		Target->ShakeViewport(2, 0, 0, 300);
		CreateParticle("SmokeDirty", PV_Random(this.x -8, this.x + 8), PV_Random(this.y -2, this.y + 2), PV_Random(-40, 40), PV_Random(-80, -30), PV_Random(10, 60), this.p2, 2);
	
		
		if (this.timer > 40)
		{
			var head = CreateObject(WormyMcWormFace, this.x, this.y, this.o)->Set(this.c);
			head->SetCategory(C4D_Vehicle);
			var a = RandomX(-20, -5);
			if(!Random(2))
				a += 25;
			head->SetVelocity(a, 100);
			head->Sound("wormy_sound", false, 90);
			Target->RemoveObject();
			return -1;
		}
	}
	
};

local next;

func Appear(length, x, y, owner)
{
	var dummy = CreateObject(Dummy, x,y, owner);
	var fx = dummy->CreateEffect(WormyAppearFx, 1, 1);
	fx.c = length;
	fx.x = x;
	fx.y = y;
	fx.o = owner;
	
	fx.p1 =
		{
			ForceY = PV_Random(-2, 2),
			ForceX = PV_Random(-2, 2),
			DampingX = 900, DampingY = 900,
			Alpha = PV_Linear(255, 0),
			R = 200, G = 150, B = 50,
			Size = PV_Linear(PV_Random(10, 20), PV_Random(40, 60)),
			Phase = PV_Random(0, 15),
			Attach = ATTACH_Front
		};
		
	fx.p2 = 
		{
			Size = PV_KeyFrames(0, 0, 0, 100, PV_Random(3, 5), 1000, 3),
			R = 180,
			G = 200,
			B = 180,
			Alpha = PV_Linear(255, 0),
			ForceY = PV_Gravity(125),
			CollisionVertex = 0
		};
}

func Set(int cnt)
{
	CreateEffect(DamageFx, 1, 1);
	
	if (cnt == 0)
	{
		SetGraphics("Face");
		return this;
	}
	
	next = CreateObject(GetID(),0,0,-1);
	var ret = next->Set(--cnt);
	var fx = CreateEffect(FollowFx, 1, 1);
	fx.n = next;
	
	return ret;
}