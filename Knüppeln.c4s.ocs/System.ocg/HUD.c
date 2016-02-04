#appendto Clonk

local hudID;

local QSymbol;
local ESymbol;
local RSymbol;
local SymbolContainer;

func Initialize(...)
{
	//SymbolContainer = CreateObject(Dummy);
	QSymbol = CreateObject(HUDSymbol, 0, 0,GetOwner());
	QSymbol->SetGraphics("Q");
	//QSymbol->Enter(SymbolContainer);
	
	
	ESymbol = CreateObject(HUDSymbol, 0, 0,GetOwner());
	ESymbol->SetGraphics("E");
	ESymbol.Visiblity = VIS_None;
	//ESymbol->Enter(SymbolContainer);
	
	RSymbol = CreateObject(HUDSymbol, 0, 0,GetOwner());
	RSymbol->SetGraphics("R");
	RSymbol.Visiblity = VIS_None;
	//RSymbol->Enter(SymbolContainer);
	
	ShowHUD();
	AddEffect("UpdateHUD", this, 1, 1, this);
	
	return _inherited(...);
}

func ShowHUD()
{
	var hud =
	{
		Player=GetOwner(),
		Top = "92%",
		Left="50%",
		Right="50%",
		Style = GUI_Multiple | GUI_NoCrop,
		
		q_icon = 
		{
			Left = "-8em",
			Right = "-4em",
			Symbol=QSymbol,
		},
		
		w_icon = 
		{
			Left = "-2em",
			Right = "2em",
			Symbol=ESymbol,
		},
		
		r_icon = 
		{
			Left = "4em",
			Right = "8em",
			Symbol=RSymbol,
		},
		
	};
	hudID = GuiOpen(hud);
	
}

func FxUpdateHUDTimer()
{
	if(GetMagicEnergy() < ChampType.Special1Spell.ManaCost || !CanCast())
		QSymbol->SetClrModulation(RGB(255,0,0));
	else
		QSymbol->SetClrModulation(RGB(255,255,255));
		
	if(GetMagicEnergy() < ChampType.Special2Spell.ManaCost || !CanCast())
		ESymbol->SetClrModulation(RGB(255,0,0));
	else
		ESymbol->SetClrModulation(RGB(255,255,255));
		
	if(GetMagicEnergy() < ChampType.Special3Spell.ManaCost || !CanCast())
		RSymbol->SetClrModulation(RGB(255,0,0));
	else
		RSymbol->SetClrModulation(RGB(255,255,255));
}

func FxUpdateHUDStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;
		
	QSymbol->RemoveObject();
	ESymbol->RemoveObject();
	RSymbol->RemoveObject();
}