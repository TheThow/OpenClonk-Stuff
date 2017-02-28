#appendto Clonk

local hudID;

local QSymbol;
local ESymbol;
local RSymbol;
local FSymbol;
local SpaceSymbol;
local SymbolContainer;

local color_spellok;
local color_spellblocked;
local color_spellcombo;

func Initialize(...)
{
	color_spellok = RGB(255,255,255);
	color_spellblocked = RGB(160,32,32);
	color_spellcombo = RGB(32,255,32);
	
	CreateSymbols();
	
	ShowHUD();
	AddEffect("UpdateHUD", this, 1, 1, this);
	
	return _inherited(...);
}

func CreateSymbols()
{
	QSymbol = CreateObject(HUDSymbol, 0, 0,GetOwner());
	QSymbol->SetGraphics("Q");
	QSymbol.Visibility = VIS_None;
	//QSymbol->Enter(SymbolContainer);
	
	ESymbol = CreateObject(HUDSymbol, 0, 0,GetOwner());
	ESymbol->SetGraphics("E");
	ESymbol.Visiblity = VIS_None;
	//ESymbol->Enter(SymbolContainer);
	
	RSymbol = CreateObject(HUDSymbol, 0, 0,GetOwner());
	RSymbol->SetGraphics("R");
	RSymbol.Visiblity = VIS_None;
	
	FSymbol = CreateObject(HUDSymbol, 0, 0,GetOwner());
	FSymbol->SetGraphics("ManF");
	FSymbol.Visiblity = VIS_None;
	
	SpaceSymbol = CreateObject(HUDSymbol, 0, 0,GetOwner());
	SpaceSymbol->SetGraphics("ManSpace");
	SpaceSymbol.Visiblity = VIS_None;
}

func UpdateSymbolsForChamp()
{
	QSymbol->SetGraphics(Format("%iQ", ChampType));
	ESymbol->SetGraphics(Format("%iE", ChampType));
	RSymbol->SetGraphics(Format("%iR", ChampType));
	FSymbol->SetGraphics(Format("%iF", ChampType));
	SpaceSymbol->SetGraphics(Format("%iSpace", ChampType));
}

func ShowHUD()
{
	var hud =
	{
		Player=GetOwner(),
		Target=this,
		Top = "100% - 4em",
		Left="50%",
		Right="50%",
		Style = GUI_Multiple | GUI_NoCrop,
		
		q_icon = 
		{
			Left = "-10em",
			Right = "-6em",
			Symbol=QSymbol,
			
			buttontext =
			{
				Text = Format("%s", GetPlayerControlAssignment(GetOwner(), CON_QuickSwitch, true, true)),
				Left = "0%",
				Top = "0%",
				Right = "100%",
				Bottom = "100%",
				Style = GUI_TextLeft | GUI_TextTop,
			},
			
			cdtext =
			{
				Text = "Over 9000!",
				Style = GUI_TextHCenter | GUI_TextVCenter,
				Priority = 100,
			},
		},
		
		e_icon = 
		{
			Left = "-6em",
			Right = "-2em",
			Symbol=ESymbol,
			
			buttontext =
			{
				Text = Format("%s", GetPlayerControlAssignment(GetOwner(), CON_Contents, true, true)),
				Left = "0%",
				Top = "0%",
				Right = "100%",
				Bottom = "100%",
				Style = GUI_TextLeft | GUI_TextTop,
			},
			
			cdtext =
			{
				Text = "Over 9000!",
				Style = GUI_TextHCenter | GUI_TextVCenter,
				Priority = 100,
			},
		},
		
		r_icon = 
		{
			Left = "-2em",
			Right = "2em",
			Symbol=RSymbol,
			
			buttontext =
			{
				Text = Format("%s", GetPlayerControlAssignment(GetOwner(), CON_NextCrew, true, true)),
				Left = "0%",
				Top = "0%",
				Right = "100%",
				Bottom = "100%",
				Style = GUI_TextLeft | GUI_TextTop,
			},
			
			cdtext =
			{
				Text = "Over 9000!",
				Style = GUI_TextHCenter | GUI_TextVCenter,
				Priority = 100,
			},
		},
		
		f_icon = 
		{
			Left = "2em",
			Right = "6em",
			Symbol=FSymbol,
			
			buttontext =
			{
				Text = Format("%s", GetPlayerControlAssignment(GetOwner(), CON_SpecialMeleeAttack, true, true)),
				Left = "0%",
				Top = "0%",
				Right = "100%",
				Bottom = "100%",
				Style = GUI_TextLeft | GUI_TextTop,
			},
			
			cdtext =
			{
				Text = "Over 9000!",
				Style = GUI_TextHCenter | GUI_TextVCenter,
				Priority = 100,
			},
		},
		
		space_icon = 
		{
			Left = "6em",
			Right = "10em",
			Symbol=SpaceSymbol,
			
			buttontext =
			{
				Text = Format("%s", GetPlayerControlAssignment(GetOwner(), CON_Interact, true, true)),
				Left = "0%",
				Top = "0%",
				Right = "100%",
				Bottom = "100%",
				Style = GUI_TextLeft | GUI_TextTop,
			},
			
			cdtext =
			{
				Text = "Over 9000!",
				Style = GUI_TextHCenter | GUI_TextVCenter,
				Priority = 100,
			},
		},
		
		hpbar =
		{
			Top = "-2em",
			Bottom = "-1em",
			Left = "-10em",
			Right = "10em",
			BackgroundColor = RGBa(64,0,0,128),
			
			hpval =
			{
				Left = "0%",
				Right = "50%",
				Top = "0%",
				Bottom = "100%",
				BackgroundColor = RGBa(255,0,0,128),
			},
			
			hptext = 
			{
				Style = GUI_TextHCenter | GUI_TextVCenter,
				Text = "100 / 100",
				Priority = 100,
			},
		},
		
		mpbar =
		{
			Top = "-1em",
			Bottom = "0em",
			Left = "-10em",
			Right = "10em",
			BackgroundColor = RGBa(0,0,64,128),
			
			mpval =
			{
				Left = "0%",
				Right = "50%",
				Top = "0%",
				Bottom = "100%",
				BackgroundColor = RGBa(0,0,255,128),
			},
			
			mptext = 
			{
				Style = GUI_TextHCenter | GUI_TextVCenter,
				Text = "100 / 100",
				Priority = 100,
			},
		},
	
	};
	hudID = GuiOpen(hud);
	
}

func RemoveHUD()
{
	GuiClose(hudID);
}

func FxUpdateHUDTimer(object target)
{
	if(ChampType == ComboMan)
	{
		if(this.SpellCombo == 1)
			QSymbol->SetClrModulation(color_spellcombo);
		else
			QSymbol->SetClrModulation(color_spellok);
			
		if(this.SpellCombo == 2)
			ESymbol->SetClrModulation(color_spellcombo);
		else
			ESymbol->SetClrModulation(color_spellok);
			
		if(this.SpellCombo == 3)
			RSymbol->SetClrModulation(color_spellcombo);
		else
			RSymbol->SetClrModulation(color_spellok);
		
		return;
	}

	if (!ChampType.Special1Spell || GetMagicEnergy() < ChampType.Special1Spell.ManaCost || !CanCast() || GetEffect("Special1CD", this) || !ChampType->CanCastSpecial1(this))
		QSymbol->SetClrModulation(color_spellblocked);
	else
		QSymbol->SetClrModulation(color_spellok);
		
	if (!ChampType.Special2Spell || GetMagicEnergy() < ChampType.Special2Spell.ManaCost || !CanCast() || GetEffect("Special2CD", this) || !ChampType->CanCastSpecial2(this))
		ESymbol->SetClrModulation(color_spellblocked);
	else
		ESymbol->SetClrModulation(color_spellok);
		
	if (!ChampType.Special3Spell || GetMagicEnergy() < ChampType.Special3Spell.ManaCost || !CanCast() || GetEffect("Special3CD", this) || !ChampType->CanCastSpecial3(this))
		RSymbol->SetClrModulation(color_spellblocked);
	else
		RSymbol->SetClrModulation(color_spellok);
	
	if(!CanCast() || GetEffect("SpecialMeleeCD", this))
		FSymbol->SetClrModulation(color_spellblocked);
	else
		FSymbol->SetClrModulation(color_spellok);
	
	if(GetEffect("BlockingCD", this))
		SpaceSymbol->SetClrModulation(color_spellblocked);
	else
		SpaceSymbol->SetClrModulation(color_spellok);
	
	
	var cdq = GetEffect("Special1CD", this);
	var cdqtext;
	if (cdq)
		cdqtext = GetHumanTime(ChampType.Special1Cooldown - cdq.Time);
	else
		cdqtext = "";
	
	var cde = GetEffect("Special2CD", this);
	var cdetext;
	if (cde)
		cdetext = GetHumanTime(ChampType.Special2Cooldown - cde.Time);
	else
		cdetext = "";
	
	var cdr = GetEffect("Special3CD", this);
	var cdrtext;
	if (cdr)
		cdrtext = GetHumanTime(ChampType.Special3Cooldown - cdr.Time);
	else
		cdrtext = "";
	
	var cdf = GetEffect("SpecialMeleeCD", this);
	var cdftext;
	if (cdf)
		cdftext = GetHumanTime(SPECIAL_MELEE_CD - cdf.Time);
	else
		cdftext = "";
	
	
	var cdspace = GetEffect("BlockingCD", this);
	var cdspacetext;
	if (cdspace)
		cdspacetext = GetHumanTime(BLOCK_CD - cdspace.Time);
	else
		cdspacetext = "";
	
	// GUI Update
	var update = 
	{
		q_icon =
		{
			cdtext =
			{
				Text = cdqtext,
			}
		},
		
		e_icon =
		{
			cdtext =
			{
				Text = cdetext,
			}
		},
		
		r_icon =
		{
			cdtext =
			{
				Text = cdrtext,
			}
		},
		
		f_icon =
		{
			cdtext =
			{
				Text = cdftext,
			}
		},
		
		space_icon =
		{
			cdtext =
			{
				Text = cdspacetext,
			}
		},
		
		hpbar = 
		{
			hpval =
			{
				Right = Format("%d%", GetEnergy() * 100 / GetMaxEnergy()),
			},
			
			hptext =
			{
				Text = Format("%d / %d", GetEnergy(), GetMaxEnergy()),
			}
		},
		
		mpbar = 
		{
			mpval =
			{
				Right = Format("%d%", GetMagicEnergy() * 100 / GetMaxMagicEnergy()),
			},
			
			mptext =
			{
				Text = Format("%d / %d", GetMagicEnergy(), GetMaxMagicEnergy()),
			},
		},
	};
	
	GuiUpdate(update, hudID);
	
	// Uncomment to show Name Tags
	//CustomMessage(Format("{{%i}} <c %x>%s</c>", ChampType, GetPlayerColor(target->GetOwner()), GetPlayerName(target->GetOwner())), target, NO_OWNER, 0, 0);
}

func GetHumanTime(int Ftime)
{
	var retval = Ftime * 10 / 36;
	return Format("%d.%ds", retval / 10, retval % 10);
}

func FxUpdateHUDStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;
	
	QSymbol->RemoveObject();
	ESymbol->RemoveObject();
	RSymbol->RemoveObject();
	FSymbol->RemoveObject();
}