#appendto Clonk

local MaxEnergy = 100000;
local MaxMagic = 100000;

local TIMER = 10;
local MOVEMENT_CD = 15;

local BLOCK_CD = 35;
local BLOCK_DURR = 4;
local BLOCK_RANGE = 25;

local JUMP_MANA = 10;
local MaxContentsCount = 1;

local healthregen_base = 100;
local HEALTHREGEN_MAX = 20;

local SPECIAL1_MANA = 25;

local choosemenu_id;

local ChampType = Rock;

local special_active =  [0, 0, 0, 0];

local RangeDummy = 0;

func Initialize()
{
	AddEffect("ManaRegen", this, 20, 5, this, Clonk);
	AddEffect("AutoHeal", this, 20, 40*3, this, Clonk);
	
	return _inherited();
}


public func ObjectControl(int plr, int ctrl, int x, int y, int strength, bool repeat, bool release)
{	
	if(IsCharging())
		return 1;
	
	if (ctrl == CON_Interact && release == false && !Contained())
	{
		if(!GetEffect("BlockingCD", this))
		{
			AddEffect("Blocking", this, 1, BLOCK_DURR, this, GetID());
			AddEffect("BlockingCD", this, 1, BLOCK_CD);
		}
	}
	
	if (ctrl == CON_QuickSwitch)
	{
		special_active[1] = !release;
		
		if(!CanCast())
			return 1;
		
		var a = GetPlayerCursorPos(plr, true);
		var x1 = a[0] - GetX();
		var y1 = a[1] - GetY();
		
		LaunchSpecial1(x1, y1, release, false);
		
		return true;
	}
	
	if (ctrl == CON_Contents)
	{
		special_active[2] = !release;
		
		if(!CanCast())
			return 1;
	
		var a = GetPlayerCursorPos(plr, true);
		var x1 = a[0] - GetX();
		var y1 = a[1] - GetY();
		
		LaunchSpecial2(x1, y1, release, false);
		
		return true;
	}
	
	
	if (ctrl == CON_NextCrew)
	{
		special_active[3] = !release;
		
		if(!CanCast())
			return 1;
	
		var a = GetPlayerCursorPos(plr, true);
		var x1 = a[0] - GetX();
		var y1 = a[1] - GetY();
		
		LaunchSpecial3(x1, y1, release, false);
		
		return true;
	}
	
	if (ctrl == CON_Use && release == false)
	{
		for(var i = 0; i < GetLength(special_active); i++)
		{
			if(special_active[i] == true)
			{
				if(CanCast())
					Call(Format("LaunchSpecial%d", i), x, y, false, true);
				
				return 1;
			}
		}
	}

	if (ctrl == CON_Jump && IsJumping() && release == false) 
	{
		if (GetEffect("IntControlJumpDouble", this))
  		{
  			if(GetMagicEnergy() >= JUMP_MANA)
  			{
  				RemoveEffect("IntControlJumpDouble", this);
	 			SetYDir(-this.JumpSpeed /4*3 * GetCon(), 100 * 100);
	 			JumpEffect("Up");
	 			DoMagicEnergy(-JUMP_MANA);
	 		}
			else
			{
				Sound("UI::Error", 0, 50, GetOwner());
			}
	 	}
	 	
	 	if(!GetEffect("JumpCD", this))
	 	{
	 		AddEffect("IntControlJumpDouble", this, 1, TIMER);
	 		AddEffect("JumpCD", this, 1, MOVEMENT_CD);
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
    		}
    		else
			{
				Sound("UI::Error", 0, 50, GetOwner());
			}
  		}
  		
  		
  		if(!GetEffect("LeftCD", this))
	 	{
	 		AddEffect("IntControlLeftDouble", this, 1, TIMER);
	 		AddEffect("LeftCD", this, 1, MOVEMENT_CD);
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
    		}
      		else
			{
				Sound("UI::Error", 0, 50, GetOwner());
			}
  		}
  		
  		if(!GetEffect("RightCD", this))
	 	{
  			AddEffect("IntControlRightDouble", this, 1, TIMER);
  			AddEffect("RightCD", this, 1, MOVEMENT_CD);
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
  		
  		if(!GetEffect("DownCD", this))
	 	{
  			AddEffect("IntControlDownDouble", this, 1, TIMER);
  			AddEffect("DownCD", this, 1, MOVEMENT_CD);
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
	RemoveEffect("*CD", this);
	return _inherited(...);
}

func Death(...)
{
	CastObjects(Flesh, 8, 50);
	//CastPXS("Blood", 50, 30);
	Sound("kill", false, 100);
	//RemoveObject();
	SetClrModulation(RGBa(255,255,255,0));
	
	return _inherited(...);
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
		
	AddEffect("StopTumbling", this, 20, 25, this, Clonk);
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

func FxBlockTimer(object target, proplist effect, int time)
{
	if(time >= BLOCK_DURR)
		return -1;
		
	Block();
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


func LaunchSpecial1(x, y, released, mouse)
{
	ChampType->LaunchSpecial1(this, x, y, released, mouse);
}

func LaunchSpecial2(int x, int y, released, mouse)
{
	ChampType->LaunchSpecial2(this, x, y, released, mouse);
}

func LaunchSpecial3(int x, int y, released, mouse)
{
	ChampType->LaunchSpecial3(this, x, y, released, mouse);
}

func LaunchSpell(id ID, x, y, x_off, y_off)
{
	if (GetMagicEnergy() >= ID.ManaCost)
	{
		var spell = CreateObject(ID,x_off, y_off,GetOwner());
		spell->Launch(this, x, y);
		DoMagicEnergy(-ID.ManaCost);
	}
	else
	{
		Sound("UI::Error", 0, 50, GetOwner());
	}
}

func ChooseMenu()
{
	var champs = [
					["Electro Man", Rock, "He is the most bestest electro man of all time", ElectroMan],
					["Fire Man", Wood, "He is the most bestest fire man of all time", FireMan],
					["Light Man", Lantern, "He is the most brightest man of all time", LightMan]
				 ];

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
	
	for(var champ in champs)
	{
		var subm =
		{
			ID = champ[1],
			Bottom = "+4em",
			icon = {Priority = 10, Symbol = champ[1], Right = "+4em", Bottom = "+4em"},
			text = {Priority = 10, Left = "+5em", Style = GUI_TextVCenter, Text = champ[0]},
			
			selector = // only visible for host
			{
				Priority = 1,
				BackgroundColor = {Std = 0, Hover = 0x50ff0000},
				
				OnMouseIn = 
					[ 
						GuiAction_Call(this, "ChampUpdateDesc", [champ[2], champ[1]]), 
						GuiAction_SetTag("Hover")
					],
			
				OnMouseOut = { Hover = GuiAction_SetTag("Std")},
				
				OnClick = GuiAction_Call(this, "SelectChamp", [champ[3]]),
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
		icon = {Symbol = data[1]},
		textwindow = {Text = data[0]}
	};
	GuiUpdate(update, choosemenu_id, 1, this);
}

func SelectChamp(data, int player, int ID, int subwindowID, object target)
{
	ChampType = data[0];
	
	this->CancelMenu();
}

func GetChampType()
{
	return ChampType;
}

func Charge(object caller, string callback, int time, proplist params, bool nosound)
{
	SetAction("Float");
	var eff = AddEffect("Charge", this, 20, time, this, GetID());
	eff.f = callback;
	eff.c = caller;
	eff.p = params;
	
	if(!nosound)
		Sound("charge", false, 20);
}

func FxChargeDamage(object target, proplist effect, int damage, int cause)
{
	if (damage >= 0)
		return;

	if(GetAction() == "Float")
		SetAction("Jump");
		
	effect.c->~ChargeInterrupted();
	RemoveEffect(effect.name, this);
}

func FxChargeStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;

	SetAction("Jump");
			
	var a = GetPlayerCursorPos(GetOwner(), true);
	var x1 = a[0] - GetX();
	var y1 = a[1] - GetY();
	
	effect.p.new_angle = Angle(0,0,x1,y1);
	
	if(effect.c)
		effect.c->Call(effect.f, effect.p);
}

func IsCharging()
{
	return GetEffect("Charge", this);
}

func CanCast()
{
	if(Contained() || GetAction() == "Tumble" || IsCharging())
		return false;
	return true;
}

func ShowSpellRange(object clonk, id spell, proplist props)
{
	CreateParticle("Shockwave", 0, 0, 0, 0, 0, props);
}

func CancelShowSpellRange()
{
	ClearParticles();
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