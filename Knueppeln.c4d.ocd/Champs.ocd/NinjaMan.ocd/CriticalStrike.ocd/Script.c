/**
	FireNado
	

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local ManaCost = 40;
local SpellRange = 40;

local SpellDamage = 50;

local Charge_dur = 15;

local angle_prec = 10;

local inner;
local outer;

func Initialize()
{
	SetAction("Travel");
	SetClrModulation(RGBa(0,0,0,0));
	
	inner =
	{
		Size = PV_Linear(1,0),
		BlitMode = GFX_BLIT_Additive,
		R = 255,
		G = 255,
		B = 255,
	};
	outer =
	{
		Size = PV_Linear(2,0),
		BlitMode = GFX_BLIT_Additive,
		R = 0,
		G = 255,
		B = 255,
	};
}

func Launch(object clonk, int x, int y)
{
	var params = {
		angle = Angle(0,0,x,y, angle_prec),
		clonk = clonk,
		x = x,
		y = y,
		//marker = marker
	};
	clonk->Charge(this, "ChargeStop", Charge_dur, params, false, true);
}

func ChargeEffect(proplist params)
{
	var clonk = params.clonk;

	for(var i = 0; i < 360; i+= RandomX(3,15))
	{
		if(!Random(10))
		{
			var props =
			{
				Size = PV_Linear(2,0),
				BlitMode = GFX_BLIT_Additive,
				R = 0,
				G = 255,
				B = 255,
				Attach = ATTACH_Back | ATTACH_MoveRelative
			};
			
			var xdir = Sin(i + 180, RandomX(10, 20));
			var ydir = -Cos(i + 180, RandomX(10, 20));
			
			clonk->CreateParticle("Flash", Sin(i, 20), -Cos(i, 20), PV_Random(xdir -2, xdir + 2), PV_Random(ydir - 2 , ydir + 2), 20, props, 2);
		}
	}
}

func ChargeStop(proplist params)
{
	var a = params.new_angle;
	var clonk = params.clonk;
	var sword = clonk->FindContents(Sword);
	
	var length = Sword_Standard_StrikingLength;
	var arm = "R";
	var animation = Format("SwordSlash%d.%s", 1, arm);
	var animation_sword = Format("Strike%d", 1);
	
	sword->PlayWeaponAnimation(clonk, animation, 10, Anim_Linear(0, 0, clonk->GetAnimationLength(animation), length, ANIM_Remove), Anim_Const(1000));
	sword->PlayAnimation(animation_sword, 10, Anim_Linear(0, 0, GetAnimationLength(animation_sword), length, ANIM_Remove), Anim_Const(1000));
	
	var range = 60;

	for(var i = a - (range-10)*10; i < a + (range-10)*10; i++)
	{
		var x = Sin(i, SpellRange - 4, 10);
		var y = -Cos(i, SpellRange - 4, 10);
		clonk->CreateParticle("Flash", x, y, 0, 0, 20, outer, 2);
	}
	for(var i = a - (range-10)*10; i < a + (range-10)*10; i++)
	{
		var x = Sin(i, SpellRange - 5, 10);
		var y = -Cos(i, SpellRange - 5, 10);
		clonk->CreateParticle("Flash", x, y, 0, 0, 20, inner, 2);
	}
	
	var eff = AddEffect("CheckHit", clonk, 1,1, nil, GetID());
	eff.p = params;
	eff.dmg = SpellDamage;
	eff.range = SpellRange;
	
	Sound("critical_swing", false, 50);
	
	RemoveObject();
}

func FxCheckHitTimer(object target, proplist effect, int time)
{
	if(time > 2)
	{
		return -1;
	}

	var params = effect.p;
	
	var a = params.new_angle;
	var range = 60;

	for(var o in FindObjects(Find_Distance(effect.range, target->GetX(), target->GetY()), Find_Func("CanBeHit", this)))
	{
		if(o->GetOwner() == target->GetOwner())
			continue;
		
		var object_angle = Angle(target->GetX(), target->GetY(), o->GetX(), o->GetY());
		var dst = Distance(target->GetX(), target->GetY(), o->GetX(), o->GetY());
		
		//if(object_angle > (a/10-range)%360 && object_angle < (a/10+range)%360 || dst < 10)
		if((Abs(GetTurnDirection(object_angle, a/10)) < range || dst < 15) && !GetEffect("CriticalHitCD", o))
		{
			AddEffect("CriticalHitCD", o, 1, 10);
			o->Fling(Sin(a, 12, 10), -Cos(a, 12, 10));
			target->WeaponDamage(o, effect.dmg);
			target->Sound("Objects::Weapons::WeaponHit?", false, 50);
		}
	}
}



func ChargeInterrupted(params)
{
	//if(params.marker)
	//	params.marker->RemoveObject();
	RemoveObject();
}

