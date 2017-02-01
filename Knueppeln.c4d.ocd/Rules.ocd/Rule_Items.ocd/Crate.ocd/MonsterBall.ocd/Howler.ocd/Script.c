


local timerfx = new Effect
{
	Timer = func()
	{
		if (Time > 150)
		{
			//Target->Fireworks();
			//Target->Sound("quack");
			Target->RemoveObject();
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
			ScheduleCall(Target, "DoTheConfetti", 2, 20);
			Target->RandomSoundForAll();
			//Target->RemoveObject();
			return -1;
		}
	}
	
};

func Initialize()
{
	SetGraphics(nil, Dummy);
	CreateEffect(timerfx,1,1);
	var fx = CreateEffect(WormyAppearFx, 1, 1);
	
	fx.x = GetX();
	fx.y = GetY();
	
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

func RandomSoundForAll()
{
	var ran = Random(4);
	var sounds = ["howler::cough", "howler::oah", "howler::panpipes1", "howler::splp"];
	Sound(sounds[ran]);
}

public func DoTheConfetti()
{
	if (!this.confetti)
	{
		this.confetti =
		{
			CollisionVertex = 500,
			OnCollision = PC_Stop(),
			ForceX = PV_Random(-5, 5, 10),
			ForceY = PV_Gravity(100),
			Size = PV_Random(1,2),
			R = PV_Random(100,255),
			G = PV_Random(100,255),
			B = PV_Random(100,255),
			DampingX = 900, DampingY = 900
		};
	}
	CreateParticle("Flash", PV_Random(-5, 5), PV_Random(0, 2), PV_Random(-30, 30), PV_Random(-80,-20), PV_Random(100, 150), this.confetti, 15);
}