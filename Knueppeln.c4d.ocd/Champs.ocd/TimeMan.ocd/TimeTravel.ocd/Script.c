/**
	TimeTravel
	

	@author 
*/

local ManaCost = 15;
local SpellDamage = 30;
local Speed = 100;
local Size = 25;
local Dur = 400;

local pos;

func Launch(object clonk, int x, int y)
{
	if(clonk.TimeTravelMark == nil)
	{
		clonk.TimeTravelMark = [clonk->GetX(), clonk->GetY()];
		clonk.timeposfx = AddEffect("TravelPos", clonk, 1, 1, nil, GetID(), Size);
		clonk.timeposfx.size = Size;
		clonk.timeposfx.dur = Dur;
		clonk->Sound("travel_pos", false, 30, clonk->GetOwner());
		RemoveObject();
		return 1;
	}
	
	SetAction("Travel");
	TimeTravel(clonk);
}

func FxTravelPosStart(object target, proplist effect, int temporary, size)
{
	if(temporary)
		return;

	effect.props = {
		R = 200,
		G = 150,
		B = 50,
		Size = 5,
		Alpha = PV_Linear(255, 0),
		BlitMode = GFX_BLIT_Additive,
		Attach=ATTACH_Front,
	};
	var tpos = target.TimeTravelMark;
	effect.dummy = CreateObject(Dummy, tpos[0], tpos[1], target->GetOwner());
	effect.dummy.Visibility = VIS_Owner;
	
	var props = {
		R = 200,
		G = 150,
		B = 50,
		Size = size*2,
		Alpha = 128,
		BlitMode = GFX_BLIT_Additive,
		Attach=ATTACH_Front,
	};
	
	effect.dummy->CreateParticle("Flash", 0, 0, 0, 0, 0, props, 2);
}

func FxTravelPosTimer(object target, proplist effect, int time)
{
	var tpos = target.TimeTravelMark;
	
	if(tpos == nil)
		return -1;
	
	if(time > effect.dur && !GetEffect("TimeTravel", target))
	{
		target.TimeTravelMark = nil;
		return -1;
	}
	
	effect.cnt +=10;
	
	effect.dummy->CreateParticle("Flash", Cos(effect.cnt, effect.size), Sin(effect.cnt, effect.size), 0, 0, 10, effect.props, 2);
	effect.dummy->CreateParticle("Flash", Cos(effect.cnt + 180, effect.size), Sin(effect.cnt + 180, effect.size), 0, 0, 10, effect.props, 2);
	CustomMessage(Format("%d", effect.dur/40 - time/40), effect.dummy, target->GetOwner());
}

func FxTravelPosStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;
	
	effect.dummy->RemoveObject();
}

func TimeTravel(object clonk)
{
	pos = clonk.TimeTravelMark;
	clonk->Enter(this);
	AddEffect("TimeTravel", clonk, 1, 1, this);
	
	if(clonk.timeposfx)
		clonk.timeposfx.dummy.Visibility = VIS_All;
	
	var angle = Angle(0, 0, clonk->GetXDir(), clonk->GetYDir());
	
	for(var i = 0; i < 8; i++)
	{
		if(Abs(clonk->GetXDir()) < 10 && Abs(clonk->GetYDir()) < 10)
			angle = Random(360);
		this->CreateTimeTrail(20, angle - 100 + 200/8*i + RandomX(-20,20), 0, 0, nil);
	}
	
	Sound("time_travel", false, 50);
	//RemoveObject();
}

func FxTimeTravelStart(object target, proplist effect, int temporary)
{
	effect.props = {
		R = 200,
		G = 150,
		B = 50,
		Size = 3,
		Alpha = PV_Linear(255, 0),
		BlitMode = GFX_BLIT_Additive,
		Attach=ATTACH_Front,
	};
	
}

func FxTimeTravelTimer(object target, proplist effect, int time)
{
	var angle = Angle(GetX(), GetY(), pos[0], pos[1]);
	var dist = Distance(GetX(), GetY(), pos[0], pos[1]);
	SetVelocity(angle, Max(Speed, Min(Speed*2, dist)));
	
	effect.cnt +=25;
	
	var newx1 = Cos(effect.cnt, 3);
	var newy1 = Sin(effect.cnt, 3);
	
	var newx2 = Cos(effect.cnt + 180, 3);
	var newy2 = Sin(effect.cnt + 180, 3);
	
	if(effect.x1 != nil)
	{
		DrawParticleLine("Flash", effect.x1 - GetX(), effect.y1 - GetY(), newx1, newy1, 2, 0, 0, 10, effect.props);
		DrawParticleLine("Flash", effect.x2 - GetX(), effect.y2 - GetY(), newx2, newy2, 2, 0, 0, 10, effect.props);
	}
	//CreateParticle("Flash", Cos(effect.cnt, 5), Sin(effect.cnt, 5), 0, 0, 10, effect.props, 1);
	//CreateParticle("Flash", Cos(effect.cnt + 180, 5), Sin(effect.cnt + 180, 5), 0, 0, 10, effect.props, 1);
	
	effect.x1 = newx1 + GetX();
	effect.y1 = newy1 + GetY();
	effect.x2 = newx2 + GetX();
	effect.y2 = newy2 + GetY();
	
	if(Distance(GetX(), GetY(), pos[0], pos[1]) <= 5)
	{
		HitArea();
		target.TimeTravelMark = nil;
		target->Exit();
		target->SetPosition(target->GetX(), target->GetY() + 5);
		target->Unstuck();
		RemoveObject();
	}
}

func HitArea()
{
	Sound("time_travel_end", false, 50);

	var flashparticle =
	{
		Alpha = PV_Linear(255,0),
		Size = Size * 2,
		R = 255,
		G = 150,
		B = 50,
		Rotation = PV_Random(0,360),
		BlitMode = GFX_BLIT_Additive,
	};
	CreateParticle("StarSpark", 0, 0, 0, 0, 10, flashparticle, 5);
	
	var particles =
	{
		Prototype = Particles_Glimmer(),
		R = 255,
		G = 150,
		B = 50,
		Alpha = 255,
		Size = PV_Linear(5, 0),
		OnCollision = PC_Bounce(),
	};
	CreateParticle("StarSpark", 0, 0, PV_Random(-80,80), PV_Random(-80, 80), 80, particles, 35);
	
	for(var o in FindObjects(Find_Distance(Size), Find_Func("CanBeHit", this), Find_Not(Find_Func("IsBlocking"))))
	{
		var angle = Angle(GetX(), GetY(), o->GetX(), o->GetY());
		o->AddTimeHitEffect();
		o->Fling(Sin(angle, 5), -Cos(angle, 5) - 2);
		WeaponDamage(o, SpellDamage);
		
		if(!o)
			continue;
		
		if(GetEffect("TimeMark", o))
		{
			Sound("mark_hit", false, 80);
			WeaponDamage(o, TimeProjectile.MarkDamage);
		}
		
	}
}

global func CreateTimeTrail(int strength, int angle, int x, int y, int color)
{
	var e = AddEffect("TimeTrail", nil, 300, 1, nil, nil, color);
	e.x = 100*(GetX() + x);
	e.y = 100*(GetY() + y);
	e.strength = strength;
	e.curr_strength = strength;
	e.angle = angle;
}

global func FxTimeTrailStart(object target, proplist e, int temp, int color)
{
	if (temp)
		return;
	e.particles =
	{
		R = 200,
		G = 150,
		B = 50,
		Rotation = PV_Step(5, PV_Random(360)),
		Size = 5,
		BlitMode = GFX_BLIT_Additive,
		Alpha = PV_Linear(255, 0),
	};
}

global func FxTimeTrailTimer(object target, effect e, int fxtime)
{
	var strength = e.strength;
	e.curr_strength = e.curr_strength * 94 / 100;
	
	var str = e.curr_strength;
	
	var initial_speed = 100 * (strength+20)/6;
	var speed = initial_speed * str / strength;
	var angle = e.angle + RandomX(-20,20);
	var x_dir = Sin(angle, speed);
	var y_dir = -Cos(angle , speed);
	
	if (speed < 2*100) return -1;
	
	// gravity
	y_dir += GetGravity() * 10;
	
	for(var i = 0; i < 2; i++)
		CreateParticle("StarSpark", e.x/100, e.y/100, RandomX(-1,1), RandomX(-1,1), 50, e.particles, 2);
		
	// then calc next position
	e.x += x_dir;
	e.y += y_dir;
	
	if (!e.noblast)
	{
		var x_dir_blast = x_dir / 200;
		var y_dir_blast = y_dir / 200;
		CreateParticle("StarSpark", e.x/100, e.y/100, PV_Random(x_dir_blast - 2, x_dir_blast + 2), PV_Random(y_dir_blast - 2, y_dir_blast + 2), 10, e.particles, 2);
	}
	
	e.curr_strength = str;
}

local ActMap = {

	Travel = {
		Prototype = Action,
		Name = "Travel",
		Procedure = DFA_FLOAT,
		NextAction = "Travel",
		Length = 1,
		Delay = 1,
		FacetBase = 1,
		Speed=10000
	},
};