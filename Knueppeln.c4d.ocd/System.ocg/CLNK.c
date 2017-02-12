#appendto Clonk

local MaxEnergy = 100000;
local MaxMagic = 100000;

local TIMER = 10;
local MOVEMENT_CD = 12;
local MOVEMENT_X_SPEED = 32;
local TUMBLE_DUR = 25;

local BLOCK_CD = 35;
local BLOCK_DUR = 5;
local BLOCK_RANGE = 25;

local SPECIAL_MELEE_CD = 120;

local JUMP_MANA = 11;
local MaxContentsCount = 2;

local healthregen_base = 100;
local HEALTHREGEN_MAX = 20;

local choosemenu_id;

local ChampType;

local special_active =  nil;

local RangeDummy;

local bloodprops;

func Construction()
{
	ChampType = Man;

	special_active =  [0, 0, 0, 0];
	
	var interval = 5;
	if(FindObject(Find_ID(Rule_FastMana)))
	{
		interval = 3;
		BLOCK_CD = 20;
	}
	
	AddEffect("ManaRegen", this, 20, interval, this, Clonk);
	AddEffect("AutoHeal", this, 20, 40*3, this, Clonk);
	RangeDummy = CreateObject(Dummy, 0, 0, GetOwner());
	RangeDummy.Visibility = VIS_Owner;
	RangeDummy->SetAction("HangOnto", this);
	
	bloodprops =  { 
		Size = PV_Linear(PV_Random(2, 4), 0),
		CollisionVertex = 500,
		OnCollision = PC_Bounce(200),
		ForceY = PV_Gravity(300),
		R = 128,
		G = 0,
		B = 0,
	};
	
	return _inherited();
}

func IsAiming() { return true; }


public func ObjectControl(int plr, int ctrl, int x, int y, int strength, bool repeat, bool release)
{	
	if (ctrl == CON_Interact && !Contained())
	{
		if(!GetEffect("BlockingCD", this))
		{
			AddEffect("Blocking", this, 1, 1, this, GetID());
			AddEffect("BlockingCD", this, 1, BLOCK_CD);
		}
		
		return true;
	}
	
	if (ctrl == CON_InventoryShiftBackward || ctrl == CON_InventoryShiftForward)
		return 1;
	
	if (ctrl == CON_Throw && !IsCarryingHeavy() && !release)
	{	
		var item = GetItem(1);
		if (item)
		{
			if(item->~IsKnueppelItem())
			{
				item->~ControlUse(this, x, y);
			}
			else
			{
				ObjectCommand("Throw", item, x, y);
			}
			return true;
		}
	}
	
	if (ctrl == CON_ControlButton)
	{
		if(release)
			return;
			
		ChampType->CtrlPress(this);
		return true;
	}
	
	if (ctrl == CON_SpecialMeleeAttack)
	{
		var a = GetPlayerCursorPos(plr, true);
		var x1 = a[0] - GetX();
		var y1 = a[1] - GetY();
	
		var flag = ChampType->SpecialMeleeAttack(this, x1, y1, release, false, CanCast() && ChampType->CanCastSpecialMelee(this), GetEffect("SpecialMeleeCD", this));
		
		if(flag)
			AddEffect("SpecialMeleeCD", this, 1, SPECIAL_MELEE_CD, this);
		
		return 1;
	}
	
	
	if (ctrl == CON_QuickSwitch)
	{
		special_active[1] = !release;
		
		var a = GetPlayerCursorPos(plr, true);
		var x1 = a[0] - GetX();
		var y1 = a[1] - GetY();
		
		LaunchSpecial1(x1, y1, release, false, CanCast() && ChampType->CanCastSpecial1(this));
		
		return true;
	}
	
	if (ctrl == CON_Contents)
	{
		special_active[2] = !release;
	
		var a = GetPlayerCursorPos(plr, true);
		var x1 = a[0] - GetX();
		var y1 = a[1] - GetY();
		
		LaunchSpecial2(x1, y1, release, false, CanCast() && ChampType->CanCastSpecial2(this));
		
		return true;
	}
	
	
	if (ctrl == CON_NextCrew)
	{
		special_active[3] = !release;
	
		var a = GetPlayerCursorPos(plr, true);
		var x1 = a[0] - GetX();
		var y1 = a[1] - GetY();
		
		LaunchSpecial3(x1, y1, release, false, CanCast() && ChampType->CanCastSpecial3(this));
		
		return true;
	}
	
	if (ctrl == CON_Use)
	{
		if(IsCharging())
			return true;
			
		ChampType->LeftClick(this, x, y, release, CanCast());
	
		var flag = false;
		
		for(var i = 0; i < GetLength(special_active); i++)
		{
			if(special_active[i] == true)
			{
				Call(Format("LaunchSpecial%d", i), x, y, release, true, CanCast() && ChampType->Call(Format("CanCastSpecial%d", i), this));
				flag = true;
			}
		}
		
		if (flag)
			return true;
	}

	if (ctrl == CON_Jump && IsJumping()) 
	{
		if (GetEffect("IntControlJumpDouble", this))
  		{
  			if(GetMagicEnergy() >= JUMP_MANA)
  			{
  				RemoveEffect("IntControlJumpDouble", this);
	 			ControlUpDouble();
	 		}
			else
			{
				Sound("UI::Error", 0, 50, GetOwner());
			}
	 	}
	 	
	 	if(!GetEffect("JumpMCD", this))
	 	{
	 		AddEffect("IntControlJumpDouble", this, 1, TIMER);
	 		AddEffect("JumpMCD", this, 1, MOVEMENT_CD);
	 	}
	}
	
	if (ctrl == CON_Left && IsJumping() && !release)
	{
	
	  	if (GetEffect("IntControlLeftDouble", this))
  		{
  		 	if(GetMagicEnergy() >= JUMP_MANA)
  			{
	    		RemoveEffect("IntControlLeftDouble", this);
	    		ControlLeftDouble();
    		}
    		else
			{
				Sound("UI::Error", 0, 50, GetOwner());
			}
  		}
  		
  		
  		if(!GetEffect("LeftMCD", this))
	 	{
	 		AddEffect("IntControlLeftDouble", this, 1, TIMER);
	 		AddEffect("LeftMCD", this, 1, MOVEMENT_CD);
	 	}
  		
	}
	
	if (ctrl == CON_Right && IsJumping() && !release)
	{
	  	if (GetEffect("IntControlRightDouble", this))
  		{
  			if(GetMagicEnergy() >= JUMP_MANA)
  			{
    			RemoveEffect("IntControlRightDouble", this);
    			ControlRightDouble();
     		}
      		else
			{
				Sound("UI::Error", 0, 50, GetOwner());
			}
  		}
  		
  		if(!GetEffect("RightMCD", this))
	 	{
  			AddEffect("IntControlRightDouble", this, 1, TIMER);
  			AddEffect("RightMCD", this, 1, MOVEMENT_CD);
  		}
	}
	
	if (ctrl == CON_Down && IsJumping())
	{
	  	if (GetEffect("IntControlDownDouble", this))
  		{
  			if(GetMagicEnergy() >= JUMP_MANA)
  			{
    			RemoveEffect("IntControlDownDouble", this);
    			ControlDownDouble();
    		}
      		else
			{
				Sound("UI::Error", 0, 50, GetOwner());
			}
  		}
  		
  		if(!GetEffect("DownMCD", this))
	 	{
  			AddEffect("IntControlDownDouble", this, 1, TIMER);
  			AddEffect("DownMCD", this, 1, MOVEMENT_CD);
  		}
	}
	
	if(ctrl == CON_Drop)
	{
		return 1;
	}
	 
	return _inherited(plr, ctrl, x, y, strength, repeat, release);
}

func JumpEffect(dir)
{
	ChampType->JumpEffect(this, dir);
}

func ControlUpDouble()
{
	SetYDir(-this.JumpSpeed * GetCon(), 100 * 100);
	JumpEffect("Up");
	DoMagicEnergy(-JUMP_MANA);	
	return true;
}

func ControlDownDouble()
{
	SetYDir(this.JumpSpeed * GetCon(), 100 * 100);
	JumpEffect("Down");
	DoMagicEnergy(-JUMP_MANA);	
	return true;
}

func ControlLeftDouble()
{
	var penality = 0;
	if(IsCarryingHeavy())
		penality = 12;
	
	if(GetXDir() > -MOVEMENT_X_SPEED + penality)
  		SetXDir(-MOVEMENT_X_SPEED + penality);
  	SetYDir(GetYDir() - 15);
  	JumpEffect("Left");
	DoMagicEnergy(-JUMP_MANA);	
  	return true;
}

func ControlRightDouble()
{
	var penality = 0;
	if(IsCarryingHeavy())
		penality = 12;

	if(GetXDir() < MOVEMENT_X_SPEED - penality)
  		SetXDir(MOVEMENT_X_SPEED - penality);	
  	SetYDir(GetYDir() - 15);
  	JumpEffect("Right");
	DoMagicEnergy(-JUMP_MANA);	
  	return true;
}

func Hit()
{
	RemoveEffect("*MCD", this);
	return _inherited(...);
}

func Death(int killed_by)
{
	if(!Scenario->~NoFlesh())
		CastObjects(Flesh, 8, 50);
		
	ChampType->CleanUp(this);

	CreateParticle("Flash", 0, 0, PV_Random(-60, 60), PV_Random(-60, 30), 140, bloodprops, 45);
	
	//CastPXS("Blood", 50, 30);
	Sound("kill", false, 100);
	
	SetClrModulation(RGBa(255,255,255,0));
	
	if(RangeDummy)
	{
		RangeDummy->ClearParticles();
		RangeDummy->RemoveObject();
	}
		
	if(FindContents(Sword))
		FindContents(Sword)->RemoveObject();
	
	AddEffect("RemoveBody", this, 1, 80, this);
	
	return _inherited(killed_by);
}

func FxRemoveBodyStop(target)
{
	if(target)
		target->RemoveObject();
}


func FxManaRegenTimer()
{
	if(GetY() > 0)
		DoMagicEnergy(13, true, 100);
	return 0;
}

func FxAutoHealTimer()
{
	if(GetEnergy() < healthregen_base + HEALTHREGEN_MAX && GetEnergy() < GetMaxEnergy())
	{
		DoEnergy(1);
	}
		
	return 0;
}

func FxAutoHealDamage(object target, proplist effect, int damage, int cause)
{
	if(damage < 0 || healthregen_base > GetEnergy())
		healthregen_base = GetEnergy();
	
	if (damage < 0)
	{
		CreateParticle("Flash", 0, 0, PV_Random(-40, 40), PV_Random(-40, 20), 140, bloodprops, Min(40, damage / -1500));
	}
	
	return damage;
}

func StartTumble()
{
	if(GetEffect("StopTumbling", this))
		RemoveEffect("StopTumbling", this);
		
	AddEffect("StopTumbling", this, 20, TUMBLE_DUR, this, Clonk);
	return _inherited();
}

func FxStopTumblingTimer()
{
	if (GetAction() == "Tumble")
	{
		SetAction("Jump");
	}
	return -1;
}

func FxBlockingStart(object target, proplist effect)
{
	Sound("block", false, 50);
	
	ChampType->BlockEffect(this, BLOCK_RANGE);
	
	FxBlockingTimer(target, effect, 0);
	return FX_OK;
}

func FxBlockingTimer(object target, proplist effect, int time)
{
	if (time >= BLOCK_DUR)
		return FX_Execute_Kill;
		
	for (var obj in FindObjects(Find_Distance(BLOCK_RANGE), Find_Func("IsReflectable", this)))
	{
		// Change the objects direction when blocked.
		var xdir = obj->GetXDir();
		var ydir = obj->GetYDir();
		var speed = Sqrt(xdir**2 + ydir**2);
		speed = Max(5, speed);
		var angle = Angle(GetX(), GetY(), obj->GetX(), obj->GetY());
        obj->SetSpeed(Sin(angle, speed), -Cos(angle, speed));
        
        // Set controller of the object to the controller of the blocker for correct kill_tracing.
        obj->SetController(GetController());
        
        // Do callback in object that it got blocked.
        obj->~Blocked(this);
        
        // Do medal callbacks for this event.
        var medal_def = GetDefinition("Rule_Medals");
        if (medal_def)
       		medal_def->~PerformMedalCallbacks("OnAttackBlock", this, obj, effect);
	}
	return FX_OK;
}

public func IsBlocking()
{
	return GetEffect("Blocking", this);
}

public func GetBlockingRange()
{
	return BLOCK_RANGE;
}

func LaunchSpecial1(x, y, released, mouse, abletocast)
{
	//!released && !mouse && abletocast && !GetEffect("Special1CD", this) &&
	var ret = ChampType->Special1(this, x, y, released, mouse, abletocast, GetEffect("Special1CD", this));
	if (ret)
	{
		AddEffect("Special1CD", this, 1, ChampType.Special1Cooldown);
		return true;
	}
	return false;
}

func LaunchSpecial2(int x, int y, released, mouse, abletocast)
{
	var ret = ChampType->Special2(this, x, y, released, mouse, abletocast, GetEffect("Special2CD", this));
	if (ret)
	{
		AddEffect("Special2CD", this, 1, ChampType.Special2Cooldown);
		return true;
	}
	return false;
}

func LaunchSpecial3(int x, int y, released, mouse, abletocast)
{
	var ret = ChampType->Special3(this, x, y, released, mouse, abletocast, GetEffect("Special3CD", this));
	if (ret)
	{
		AddEffect("Special3CD", this, 1, ChampType.Special3Cooldown);
		return true;
	}
	return false;
}

func LaunchSpell(id ID, int x, int y, int x_off, int y_off, proplist params)
{
	var spell = nil;
	var flag = false;

	if (GetMagicEnergy() >= ID.ManaCost)
	{
		spell = CreateObject(ID,x_off, y_off,GetOwner());
		spell->SetController(GetOwner());
		spell->Launch(this, x, y, params);
		DoMagicEnergy(-ID.ManaCost);
		flag = true;
	}
	else
	{
		Sound("UI::Error", 0, 50, GetOwner());
		return false;
	}
	
	if (spell == nil)
		return flag;
	
	return spell;
}


func GetChampType()
{
	return ChampType;
}

func Charge(object caller, string callback, int time, proplist params, bool nosound, bool nofloat)
{
	/*if (ChampType == ComboMan)
	{
			var eff = AddEffect("Charge", this, 20, 1, this);
			eff.f = callback;
			eff.c = caller;
			eff.p = params;
			eff.time = time;
			eff.p.new_angle = eff.p.a;
			eff.p.new_x = eff.p.x;
			eff.p.new_y = eff.p.y;
			eff.p.time = time;
			RemoveEffect("Charge", this);
			return;
	}*/

	if(!nofloat)
		SetAction("Float");
		
	var eff = AddEffect("Charge", this, 20, 1, this);
	eff.f = callback;
	eff.c = caller;
	eff.p = params;
	eff.time = time;
	
	if(!nosound)
		Sound("charge", false, 20);
}

func FxChargeTimer(object target, proplist effect, int time)
{
	if(!effect.c)
		return -1;

	// Scriptplayers have no cursor. The KnueppelnAI has got a helperfunction for that. It returns coordinates of the AI-Clonk's Target.
	var a = GetPlayerCursorPos(GetOwner(), true);
	var x1;
	var y1;
	if (!a)
	{
		a = KnueppelnAI->GetAICursor(KnueppelnAI->GetAI(target));
		x1 = a[0];
		y1 = a[1];
	}
	else
	{
		x1 = a[0] - GetX();
		y1 = a[1] - GetY();
	}
	
	
	effect.p.new_angle = Angle(0,0,x1,y1,10);
	effect.p.new_x = x1;
	effect.p.new_y = y1;
	effect.p.time = time;

	effect.c->~ChargeEffect(effect.p);
	
	if(time >= effect.time)
		return -1;
}

func FxChargeDamage(object target, effect fx, int damage, int cause)
{
	if (damage >= 0)
		return damage;

	if(GetAction() == "Float")
		SetAction("Jump");
	
	if(fx.c)
	{
		fx.c->~ChargeInterrupted(fx.p);
		fx.c = nil;
	}	
	RemoveEffect(nil, nil, fx);
	
	return damage;
}

func FxChargeStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;
		
	if(!effect.c)
		return 0;

	SetAction("Jump");
			
	var a = GetPlayerCursorPos(GetOwner(), true);
	var x1;
	var y1;
	if (!a)
	{
		a = KnueppelnAI->GetAICursor( KnueppelnAI->GetAI(target) );
		x1 = a[0];
		y1 = a[1];
	}
	else
	{
		x1 = a[0] - GetX();
		y1 = a[1] - GetY();
	}
	
	effect.p.new_angle = Angle(0,0,x1,y1, 10);
	effect.p.new_x = x1;
	effect.p.new_y = y1;
	
	if(effect.c)
		effect.c->Call(effect.f, effect.p);
	
}

func IsCharging()
{
	return GetEffect("Charge", this);
}

func CanCast()
{
	if(Contained() || 
		GetAction() == "Tumble" || 
		IsCharging() || 
		GetAction() == "Float" || 
		GetEffect("SpawnProtection", this) || 
		!ChampType->CanCast(this) ||
		GetEffect("CastingDisabled", this) ||
		IsCarryingHeavy())
		return false;
	return true;
}

func DisableCasting(int time)
{
	AddEffect("CastingDisabled", this, 1, time, this);
}

func ShowSpellRange(object clonk, id spell, proplist props)
{
	if(RangeDummy && RangeDummy->Contained())
		RangeDummy->RemoveObject();

	if(!RangeDummy)
	{
		RangeDummy = CreateObject(Dummy, 0, 0, GetOwner());
		RangeDummy.Visibility = VIS_Owner;
		RangeDummy->SetAction("HangOnto", this);
	}

	RangeDummy->CreateParticle("Shockwave2", 0, 0, 0, 0, 0, props);
	RangeDummy.range_on = true;
}

func CancelShowSpellRange()
{
	if(RangeDummy)
	{
		RangeDummy->ClearParticles();
		RangeDummy.range_on = false;
	}
}

func CanBeHit(object from)
{
	if (GetEffect("Unhitable", this))
		return false;
		
	if (Contained())
		return false;
	
	if (!GetAlive())
		return false;
	return true;
}

func IsProjectileTarget(object from)
{
	if (!GetAlive())
		return false;

	if ((ChampType == GreatLeaderMan) && from.is_minions_arrow && (from->GetController() == GetController()))
		return false;
		
	return true;
}

func MakeHitable(bool hitable)
{
	if(hitable)
	{
		if(GetEffect("Unhitable", this))
			RemoveEffect("Unhitable", this);
	}
	else
	{
		AddEffect("Unhitable", this, 20, 1, this);
	}
}

func FxUnhitableTimer()
{
	return;
}

func FxUnhitableDamage()
{
	return;
}

func Incinerate(
	strength /* strength between 0 and 100 */
	, int caused_by /* the player that caused the incineration */
	, blasted /* whether the object was incinerated by an explosion */
	, incinerating_object /* the object that caused the incineration */)
{
	if(ChampType == FireMan && !GBackLiquid())
		return;

	caused_by = caused_by ?? (incinerating_object || this)->GetController();
	return DoEnergy(-3, nil, nil, caused_by);
}

local ActMap = {

	Float = {
		Prototype = Action,
		Name = "Float",
		Procedure = DFA_FLOAT,
		NextAction = "Float",
		Length = 1,
		Delay = 1,
		FacetBase = 1,
		Speed=0
	},
	
	Travel = {
		Prototype = Action,
		Name = "Travel",
		Procedure = DFA_FLOAT,
		NextAction = "Travel",
		Length = 1,
		Delay = 1,
		FacetBase = 1,
		Speed=1000
	}
};