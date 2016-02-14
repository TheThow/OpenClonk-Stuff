#appendto Clonk

func SelectChampion()
{
	var rotation = FindObject(Find_ID(Rule_ChampRotation));
	if(rotation)
	{
		var type = rotation->GetChamp(GetOwner());
		SelectChamp([type], GetOwner());
		PlayerMessage(GetOwner(), "%s", type.Name);
		return;
	}
	ChooseMenu();
}

func ChooseMenu()
{
	//var champs = [
	//				["Electro Man", ElectroMan],
	//				["Fire Man", FireMan],
	//				["Light Man", LightMan]
	//			 ];
	
	var champs = getChampions();
	
	if(FindObject(Find_ID(Rule_RandomChamp)))
	{
		var i = Random(GetLength(champs));
		var u = Random(GetLength(champs));
		
		while(u == i)
			u = Random(GetLength(champs));
		
		champs = [champs[i], champs[u]];
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
	
	ChampType->InitChamp(this);
	
	this->CancelMenu();
	var relauncher = Contained();
	if (relauncher)
		relauncher->~RelaunchClonk();
}