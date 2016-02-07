/**
	FireNado
	

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local ManaCost = 40;
local SpellRange = 40;

local SpellDamage = 50;
local SpellRange = 30;

local Charge_dur = 25;

local angle_prec = 10;

func Initialize()
{
	SetAction("Travel");
	SetClrModulation(RGBa(0,0,0,0));
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
				R = 255,
				G = 0,
				B = 0,
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
	var range = 60;
	
	var clonk = params.clonk;
	var sword = clonk->FindContents(Sword);
	
	var length = Sword_Standard_StrikingLength;
	var rand = Random(2)+1;
	var arm = "R";
	var animation = Format("SwordSlash%d.%s", rand, arm);
	var animation_sword = Format("Strike%d", rand);
	
	sword->PlayWeaponAnimation(clonk, animation, 10, Anim_Linear(0, 0, clonk->GetAnimationLength(animation), length, ANIM_Remove), Anim_Const(1000));
	sword->PlayAnimation(animation_sword, 10, Anim_Linear(0, 0, GetAnimationLength(animation_sword), length, ANIM_Remove), Anim_Const(1000));
	
	var flag = false;
	
	for(var i = a - (range-10)*10; i < a + (range-10)*10; i++)
	{
		var x = Sin(i, SpellRange, 10);
		var y = -Cos(i, SpellRange, 10);
		
		var props =
		{
			Size = PV_Linear(2,0),
			BlitMode = GFX_BLIT_Additive,
			R = 255,
			G = 0,
			B = 0,
		};
		
		clonk->CreateParticle("Flash", x, y, 0, 0, 20, props, 2);
	}
	
	for(var o in FindObjects(Find_Distance(SpellRange), Find_Func("CanBeHit")))
	{
		if(o->GetOwner() == GetOwner())
			continue;
		
		var object_angle = Angle(GetX(), GetY(), o->GetX(), o->GetY());
		var dst = Distance(GetX(), GetY(), o->GetX(), o->GetY());
		
		//if(object_angle > (a/10-range)%360 && object_angle < (a/10+range)%360 || dst < 10)
		if(Abs(GetTurnDirection(object_angle, a/10)) < range || dst < 12)
		{
			o->Fling(Sin(a, 12, 10), -Cos(a, 12, 10));
			WeaponDamage(o, SpellDamage);
			Sound("Objects::Weapons::WeaponHit?", false, 50);
			flag = true;
		}
	}
	
	if(!flag)
		Sound("Objects::Weapons::WeaponSwing?", false, 50);
	
	RemoveObject();
}



func ChargeInterrupted(params)
{
	//if(params.marker)
	//	params.marker->RemoveObject();
	RemoveObject();
}

