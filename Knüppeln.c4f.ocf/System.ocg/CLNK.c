#appendto Clonk

local Champ_Def = [ElectroMan, FireMan, LaserMan, IceMan, EarthMan];

local MaxEnergy = 100000;
local MaxMagic = 100000;

local TIMER = 10;
local MOVEMENT_CD = 12;
local TUMBLE_DUR = 25;

local BLOCK_CD = 35;
local BLOCK_DUR = 5;
local BLOCK_RANGE = 25;

local JUMP_MANA = 8;
local MaxContentsCount = 1;

local healthregen_base = 100;
local HEALTHREGEN_MAX = 20;

local SPECIAL1_MANA = 25;

local choosemenu_id;

local ChampType = Man;

local special_active =  nil;

local RangeDummy;

func Construction()
{
	special_active =  [0, 0, 0, 0];
	
	AddEffect("ManaRegen", this, 20, 5, this, Clonk);
	AddEffect("AutoHeal", this, 20, 40*3, this, Clonk);
	RangeDummy = CreateObject(Dummy, 0, 0, GetOwner());
	RangeDummy.Visibility = VIS_Owner;
	RangeDummy->SetAction("HangOnto", this);
	
	return _inherited();
}


public func ObjectControl(int plr, int ctrl, int x, int y, int strength, bool repeat, bool release)
{	
	if (plr != GetOwner())
	{
		Log("%s (%s) got event of %s", GetPlayerName(GetOwner()), GetName(), GetPlayerName(plr));
		return false;
	}
	
	if (ctrl == CON_Interact && release == false && !Contained())
	{
		if(!GetEffect("BlockingCD", this))
		{
			AddEffect("Blocking", this, 1, 1, this, GetID());
			AddEffect("BlockingCD", this, 1, BLOCK_CD);
		}
	}
	
	if (ctrl == CON_QuickSwitch)
	{
		special_active[1] = !release;
		
		var a = GetPlayerCursorPos(plr, true);
		var x1 = a[0] - GetX();
		var y1 = a[1] - GetY();
		
		LaunchSpecial1(x1, y1, release, false, CanCast());
		
		return true;
	}
	
	if (ctrl == CON_Contents)
	{
		special_active[2] = !release;
	
		var a = GetPlayerCursorPos(plr, true);
		var x1 = a[0] - GetX();
		var y1 = a[1] - GetY();
		
		LaunchSpecial2(x1, y1, release, false, CanCast());
		
		return true;
	}
	
	
	if (ctrl == CON_Skill3)
	{
		if(special_active[3] == !release)
		{
			Log("Missing event");
		}
		special_active[3] = !release;
		
		if(special_active[3] != !release)
			Log("ARRAYS FUCKED UP");
	
		var a = GetPlayerCursorPos(plr, true);
		var x1 = a[0] - GetX();
		var y1 = a[1] - GetY();
		
		LaunchSpecial3(x1, y1, release, false, CanCast());
		
		return true;
	}
	
	if (ctrl == CON_Use)
	{
		var flag = false;
		
		for(var i = 0; i < GetLength(special_active); i++)
		{
			if(special_active[i] == true)
			{
				Call(Format("LaunchSpecial%d", i), x, y, release, true, CanCast());
				flag = true;
			}
		}
		
		if (flag)
			return true;
	}

	if (ctrl == CON_Jump && IsJumping() && release == false) 
	{
		if (GetEffect("IntControlJumpDouble", this))
  		{
  			if(GetMagicEnergy() >= JUMP_MANA)
  			{
  				RemoveEffect("IntControlJumpDouble", this);
	 			SetYDir(-this.JumpSpeed*4/5 * GetCon(), 100 * 100);
	 			JumpEffect("Up");
	 			DoMagicEnergy(-JUMP_MANA);
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
	
	if (ctrl == CON_Left && IsJumping() && release == false)
	{
	
	  	if (GetEffect("IntControlLeftDouble", this))
  		{
  		 	if(GetMagicEnergy() >= JUMP_MANA)
  			{
	    		RemoveEffect("IntControlLeftDouble", this);
	    		ControlLeftDouble();
	    		JumpEffect("Left");
	    		DoMagicEnergy(-JUMP_MANA);
	    		
	    		if(GetDir() == DIR_Right)
					FlipDir();
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
	
	if (ctrl == CON_Right && IsJumping() && release == false)
	{
	  	if (GetEffect("IntControlRightDouble", this))
  		{
  			if(GetMagicEnergy() >= JUMP_MANA)
  			{
    			RemoveEffect("IntControlRightDouble", this);
    			ControlRightDouble();
    			JumpEffect("Right");
    			DoMagicEnergy(-JUMP_MANA);
    			
    			if(GetDir() == DIR_Left)
					FlipDir();
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
	
	if (ctrl == CON_Down && IsJumping() && release == false)
	{
	  	if (GetEffect("IntControlDownDouble", this))
  		{
  			if(GetMagicEnergy() >= JUMP_MANA)
  			{
    			RemoveEffect("IntControlDownDouble", this);
    			SetYDir(this.JumpSpeed * GetCon(), 100 * 100);
    			JumpEffect("Down");
    			DoMagicEnergy(-JUMP_MANA);
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


func ControlLeftDouble()
{
  	SetXDir(-40);
  	SetYDir(GetYDir() - 15);
  	return true;
}

func ControlRightDouble()
{
  	SetXDir(40);
  	SetYDir(GetYDir() - 15);
  	return true;
}

func Hit()
{
	RemoveEffect("*MCD", this);
	return _inherited(...);
}

func Death(int killed_by)
{
	CastObjects(Flesh, 8, 50);
	//CastPXS("Blood", 50, 30);
	Sound("kill", false, 100);
	
	SetClrModulation(RGBa(255,255,255,0));
	
	if(RangeDummy)
	{
		RangeDummy->ClearParticles();
		RemoveObject(RangeDummy);
	}
	
	if(this)
		FadeOut(30, true);
	
	return _inherited(killed_by);
}

func FxManaRegenTimer()
{
	DoMagicEnergy(1);
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

func FxBlockingStart()
{
	Sound("block", false, 50);
	
	ChampType->BlockEffect(this, BLOCK_RANGE);
	
	Block();
}

func FxBlockingTimer(object target, proplist effect, int time)
{
	if(time >= BLOCK_DUR)
		return -1;
		
	Block();
}

public func IsBlocking()
{
	return GetEffect("Blocking", this) != nil;
}

public func GetBlockingRange()
{
	return BLOCK_RANGE;
}

func Block()
{
	for(var o in FindObjects(Find_Distance(BLOCK_RANGE), Find_Func("IsReflectable")))
	{
		var xdir = o->GetXDir();
		var ydir = o->GetYDir();
		
		//var entryangle = Angle(0,0,xdir,ydir);
		var objectangle = Angle(GetX(), GetY(), o->GetX(), o->GetY());
		
		var speed = Sqrt(xdir**2+ydir**2);
		
		//var tangle = 2* ( (objectangle + 90)%360 - entryangle) + entryangle;
		var tangle = objectangle;
        o->SetSpeed(Sin(tangle, speed), -Cos(tangle, speed));
        
        o->~Blocked();
	}
}


func LaunchSpecial1(x, y, released, mouse, abletocast)
{
	ChampType->Special1(this, x, y, released, mouse, abletocast, GetEffect("Special1CD", this));
	if(!released && !mouse && abletocast && !GetEffect("Special1CD", this))
		AddEffect("Special1CD", this, 20, ChampType.Special1Cooldown);
	
}

func LaunchSpecial2(int x, int y, released, mouse, abletocast)
{
	ChampType->Special2(this, x, y, released, mouse, abletocast, GetEffect("Special2CD", this));
	if(!released && !mouse && abletocast && !GetEffect("Special2CD", this))
		AddEffect("Special2CD", this, 20, ChampType.Special2Cooldown);
}

func LaunchSpecial3(int x, int y, released, mouse, abletocast)
{
	ChampType->Special3(this, x, y, released, mouse, abletocast, GetEffect("Special3CD", this));
	if(!released && !mouse && abletocast && !GetEffect("Special3CD", this))
		AddEffect("Special3CD", this, 20, ChampType.Special3Cooldown);
}

func LaunchSpell(id ID, x, y, x_off, y_off)
{
	var spell;

	if (GetMagicEnergy() >= ID.ManaCost)
	{
		spell = CreateObject(ID,x_off, y_off,GetOwner());
		spell->SetController(GetOwner());
		spell->Launch(this, x, y);
		DoMagicEnergy(-ID.ManaCost);
	}
	else
	{
		Sound("UI::Error", 0, 50, GetOwner());
	}
	
	return spell;
}

func ChooseMenu()
{
	//var champs = [
	//				["Electro Man", ElectroMan],
	//				["Fire Man", FireMan],
	//				["Light Man", LightMan]
	//			 ];
	
	var champs = Champ_Def;
	
	if(FindObject(Find_ID(Rule_RandomChamp)))
	{
		var i = Random(GetLength(Champ_Def));
		var u = Random(GetLength(Champ_Def));
		
		while(u == i)
			u = Random(GetLength(Champ_Def));
		
		champs = [Champ_Def[i], Champ_Def[u]];
	}

	var menu = 
	{
		Player = GetOwner(),
	
		list = 
		{
			Right = "50%",
			Margin = ["2em", "2em"],
			Style = GUI_VerticalLayout,
			BackgroundColor = RGBa(0, 0, 0, 150)
		},
		right = {
			Target = this,
			Left = "50%",
			Margin = ["2em", "2em"],
			Decoration = GUI_MenuDeco,
			ID = 1,
			icon = {Left="50%-4em", Right="50%+4em", Bottom="5em", Top="1em", Symbol=Clonk},
			textwindow =
			{
				Top = "6em",
				Left = "10%",
				Right = "80%",
				Text = "Select your Champion!"
			}
		}
	};
	
	var index = 0;
	for(var champ in champs)
	{
		index += 1;
		var subm =
		{
			ID = champ,
			Priority = index,
			Bottom = "+4em",
			icon = {Priority = 10, Symbol = champ, Right = "+4em", Bottom = "+4em"},
			text = {Priority = 10, Left = "+5em", Style = GUI_TextVCenter, Text = champ.Name},
			
			selector = // only visible for host
			{
				Priority = 1,
				BackgroundColor = {Std = 0, Hover = 0x50ff0000},
				
				OnMouseIn = 
					[ 
						GuiAction_Call(this, "ChampUpdateDesc", [champ]), 
						GuiAction_SetTag("Hover")
					],
			
				OnMouseOut = { Hover = GuiAction_SetTag("Std")},
				
				OnClick = GuiAction_Call(this, "SelectChamp", [champ]),
			},
		};
		GuiAddSubwindow(subm, menu.list);
	}
	
	choosemenu_id = GuiOpen(menu);
	this->SetMenu(choosemenu_id, true);
}

func ChampUpdateDesc(data, int player, int ID, int subwindowID, object target)
{
	var update = 
	{
		icon = {Symbol = data[0]},
		textwindow = {Text = data[0].Description}
	};
	GuiUpdate(update, choosemenu_id, 1, this);
}

func SelectChamp(data, int player, int ID, int subwindowID, object target)
{
	ChampType = data[0];
	
	this->CancelMenu();
	var relauncher = Contained();
	if (relauncher)
		relauncher->~RelaunchClonk();
}

func GetChampType()
{
	return ChampType;
}

func Charge(object caller, string callback, int time, proplist params, bool nosound)
{
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

	var a = GetPlayerCursorPos(GetOwner(), true);
	var x1 = a[0] - GetX();
	var y1 = a[1] - GetY();
	
	effect.p.new_angle = Angle(0,0,x1,y1,10);
	effect.p.new_x = x1;
	effect.p.new_y = y1;

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
	var x1 = a[0] - GetX();
	var y1 = a[1] - GetY();
	
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
	if(Contained() || GetAction() == "Tumble" || IsCharging() || GetAction() == "Float" || GetEffect("SpawnProtection", this))
		return false;
	return true;
}

func ShowSpellRange(object clonk, id spell, proplist props)
{
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

func CanBeHit()
{
	if(GetEffect("Unhitable", this))
		return false;
		
	if(Contained())
		return false;
	
	if(!GetAlive())
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
		StartCall = "Floating",
		Speed=0
	}
};