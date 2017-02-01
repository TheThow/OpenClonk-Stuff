#appendto Clonk

func SelectChampion()
{
	if(FindObject(Find_ID(Rule_InstaGib)))
	{
		SelectChamp([InstaGibMan], GetOwner());
		return;
	}
	
	if(FindObject(Find_ID(Rule_ComboMode)))
	{
		SelectChamp([ComboMan], GetOwner());
		return;
	}

	var rotation = FindObject(Find_ID(Rule_ChampRotation));
	if(rotation)
	{
		var type = rotation->GetChamp(GetOwner());
		SelectChamp([type], GetOwner());
		PlayerMessage(GetOwner(), "<i>Playing as:</i>|{{%i}}  %s", type, type.Name);
		return;
	}
	
	if (GetPlayerType(GetOwner()) == C4PT_Script) {	AIChooseChamp(); }
	else ChooseMenu();
}

func ChooseMenu()
{
	var champs = GetChampions();
	
	if(FindObject(Find_ID(Rule_RandomChamp)))
	{
		champs = GetChampionsWithoutBanned();
		var i = Random(GetLength(champs));
		var u = Random(GetLength(champs));
		
		while(u == i)
		{
			u = Random(GetLength(champs));
		}
		
		champs = [champs[i], champs[u]];
	}

	var menu = 
	{
		Player = GetOwner(),
		Decoration = GUI_MenuDeco,
		Margin = ["2em", "2em"],
		list = 
		{
			Right = "50%-0.5em",
			Style = GUI_GridLayout,
			BackgroundColor = RGBa(0, 0, 0, 150)
		},
		right = {
			Target = this,
			Left = "50%+0.5em",
			ID = 1,
			icon = 
			{
				Left = "0%",
				Right = "100%",
				Bottom = "5em",
				Top = "1em",
				/* Symbol = Clonk, */
				
				champicon =
				{
					Symbol = Sword,
					Left = "1em",
					Right = "5em",
				},
				
				champname =
				{
					Left = "7em",
					Right = "100%",
					Text = "",
					Style = GUI_TextVCenter,
				}
				
			},
			textwindow =
			{
				Top = "6em",
				Left = "10%",
				Right = "80%",
				Text = "Select your Champion!"
			}
		},
		separator = {
			Left = "50%-0.5em",
			Right = "50%+0.5em",
			BackgroundColor = RGBa(240, 240, 240, 150)
		}
	};
	
	var index = 0;
	for (var champ in champs)
	{
		index += 1;
		var subm =
		{
			ID = 100 + index,
			Priority = index,
			Bottom = "+2.5em",
			Right = "+2.5em",
			BackgroundColor = {Std = 0, Hover = 0x50ffff00, Nope = 0x50ff0000},
			OnMouseIn = [ 
				GuiAction_Call(this, "ChampUpdateDesc", [champ]), 
				GuiAction_SetTag("Hover")
			],
			OnMouseOut = GuiAction_SetTag("Std"),
			OnClick = GuiAction_Call(this, "SelectChamp", [champ]),
			icon = {
				Symbol = champ,
				Right = "+2.5em"
			},
			/* text = {
				Left = "+2.5em", 
				Style = GUI_TextVCenter, 
				Text = champ.Name
			}, */
		};
		GuiAddSubwindow(subm, menu.list);
	}
	
	choosemenu_id = GuiOpen(menu);
	this->SetMenu(choosemenu_id, true);
	
	UpdateSelectionMenu();
}

func ChampUpdateDesc(data, int player, int ID, int subwindowID, object target)
{
	var update = 
	{
		icon = 
		{
			champicon =
			{
				Symbol = data[0],
			},
			
			champname = 
			{
				Text = data[0].Name,
			},
		},
		textwindow = {Text = data[0].Description}
	};
	GuiUpdate(update, choosemenu_id, 1, this);
}

func AIChooseChamp()
{
	var rndstuff = Random(2);
	if (rndstuff == 0) ScheduleCall(this, "SelectChamp", Random(150), 0, [FireMan]);
	if (rndstuff == 1) ScheduleCall(this, "SelectChamp", Random(150), 0, [ElectroMan]);
}

func SelectChamp(data, int player, int ID, int subwindowID, object target)
{	
	if(FindObject(Find_Func("UseTeamExclusiveChampions")) && !FindObject(Find_ID(Rule_InstaGib)))
	{
		var banned = GetCurrentBannedTeamChampions(GetPlayerTeam(GetOwner()));
		
		for(var c in banned)
		{
			if(c == data[0])
			{
				Sound("UI::Error", true, 50, player);
				return false;
			}
		}
		
		//RemoveTeamChamp(GetPlayerTeam(GetOwner()), ChampType);
	}
	
	for(var c in GetBannedChampions())
	{
		if(c == data[0])
		{
			Sound("UI::Error", true, 50, player);
			return false;
		}
	}
	
	ChampType = data[0];
	ChampType->InitChamp(this);
	
	this->CancelMenu();
	
	SetLastChosenChampion(player, ChampType);
	
	if(!FindObject(Find_Func("NoRelaunchReleaseOnSelection")))
	{
		var relauncher = Contained();
		if (relauncher)
			relauncher->~RelaunchClonk();
	}
	
	UpdateAllSelectionMenus();
	UpdateSymbolsForChamp();
}

func UpdateSelectionMenu()
{
	var possible = GetChampions();

	if(FindObject(Find_Func("UseTeamExclusiveChampions")))
	{
		var banned = GetCurrentBannedTeamChampions(GetPlayerTeam(GetOwner()));
		for(var c in banned)
		{
			var champ_index = GetIndexOf(possible, c) + 101;
			GuiUpdate({BackgroundColor = 0x50ff0000}, choosemenu_id, champ_index, nil);
			//GuiUpdateTag("Nope", choosemenu_id, c);
		}
	}
	
	for(var c in GetBannedChampions())
	{
		var champ_index = GetIndexOf(possible, c) + 101;
		GuiUpdate({BackgroundColor = 0x50ff0000}, choosemenu_id, champ_index, nil);
		//GuiUpdateTag("Nope", choosemenu_id, c);
	}
}