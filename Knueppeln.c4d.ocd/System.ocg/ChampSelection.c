#appendto Clonk

func SelectChampion()
{
	if(FindObject(Find_ID(Rule_InstaGib)))
	{
		SelectChamp(InstaGibMan, GetOwner());
		return;
	}
	
	if(FindObject(Find_ID(Rule_ComboMode)))
	{
		SelectChamp(ComboMan, GetOwner());
		return;
	}

	var rotation = FindObject(Find_ID(Rule_ChampRotation));
	if (rotation)
	{
		var type = rotation->GetChamp(GetOwner());
		SelectChamp(type, GetOwner());
		PlayerMessage(GetOwner(), "<i>Playing as:</i>|{{%i}}  %s", type, type.Name);
		return;
	}
	
	if (GetPlayerType(GetOwner()) == C4PT_Script)
		AIChooseChamp();
	else ChooseMenu();
}

func ChooseMenu()
{
	var champs = GetChampions();
	
	if (FindObject(Find_ID(Rule_RandomChamp)))
	{
		champs = GetChampionsWithoutBanned();
		var i = Random(GetLength(champs));
		var u = Random(GetLength(champs));
		
		while (u == i)
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
			Right = "50%-0.4em",
			Style = GUI_GridLayout,
			BackgroundColor = RGBa(0, 0, 0, 150)
		},
		right = {
			Target = this,
			Left = "50%+0.4em",
			ID = 1,
			champ = 
			{
				Top = "0em",
				Bottom = "5em",
				Margin = "0.1em",
				icon =
				{
					Symbol = nil,
					Right = "5em",
				},
				text =
				{
					Left = "5em",
					Margin = ["0.1em", nil, nil, nil],
					Text = "Select your Champion!",
				}				
			},
			champspell1 = 
			{
				Top = "5em",
				Bottom = "8em",
				Margin = "0.1em",
				icon = 
				{
					Right = "3em",
					Symbol = nil,
				},
				text = 
				{
					Left = "3em",
					Margin = ["0.1em", nil, nil, nil],
					Text = nil				
				}
			},
			champspell2 = 
			{
				Top = "8em",
				Bottom = "11em",
				Margin = "0.1em",
				icon = 
				{
					Right = "3em",
					Symbol = nil,
				},
				text = 
				{
					Left = "3em",
					Margin = ["0.1em", nil, nil, nil],
					Text = nil				
				}
			},
			champspell3 = 
			{
				Top = "11em",
				Bottom = "14em",
				Margin = "0.1em",
				icon = 
				{
					Right = "3em",
					Symbol = nil,
				},
				text = 
				{
					Left = "3em",
					Margin = ["0.1em", nil, nil, nil],
					Text = nil				
				}
			}
		},
		separator = 
		{
			Left = "50%-0.4em",
			Right = "50%+0.4em",
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
			Bottom = "4em",
			Right = "4em",
			BackgroundColor = {Std = 0, Hover = 0x50ffff00, Nope = 0x50ff0000},
			OnMouseIn = 
			[ 
				GuiAction_Call(this, "ChampUpdateDesc", champ), 
				GuiAction_SetTag("Hover")
			],
			OnMouseOut = GuiAction_SetTag("Std"),
			OnClick = GuiAction_Call(this, "SelectChamp", champ),
			icon = 
			{
				Symbol = champ
			},
			/*text = {
				Top = "3em", 
				Style = GUI_TextHCenter | GUI_TextVCenter, 
				Text = champ.Name
			}*/
		};
		GuiAddSubwindow(subm, menu.list);
	}
	
	choosemenu_id = GuiOpen(menu);
	this->SetMenu(choosemenu_id, true);
	
	UpdateSelectionMenu();
}

func ChampUpdateDesc(id champ, int player, int ID, int subwindowID, object target)
{
	var update = 
	{
		champ = 
		{
			icon = 
			{
				Symbol = champ
			},			
			text = 
			{
				Text = Format("<c aa0000>%s:</c> %s", champ.Name, champ.Description)
			},
		},
		champspell1 =
		{
			icon =
			{
				Symbol = HUDSymbol,
				GraphicsName = Format("%iQ", champ),
			},
			text = 
			{
				Text = Format("<c aa0000>%s:</c> %s", champ.Spell1Name, champ.Spell1Description)
			}
		},
		champspell2 =
		{
			icon =
			{
				Symbol = HUDSymbol,
				GraphicsName = Format("%iE", champ),
			},
			text = 
			{
				Text = Format("<c aa0000>%s:</c> %s", champ.Spell2Name, champ.Spell2Description)
			}
		},
		champspell3 =
		{
			icon =
			{
				Symbol = HUDSymbol,
				GraphicsName = Format("%iR", champ),
			},
			text = 
			{
				Text = Format("<c aa0000>%s:</c> %s", champ.Spell3Name, champ.Spell3Description)
			}
		}
	};
	GuiUpdate(update, choosemenu_id, 1, this);
}

func AIChooseChamp()
{
	ScheduleCall(this, "SelectChamp", Random(150), 0, RandomElement([FireMan, ElectroMan]));
}

func SelectChamp(id champ, int player, int ID, int subwindowID, object target)
{	
	if (FindObject(Find_Func("UseTeamExclusiveChampions")) && !FindObject(Find_ID(Rule_InstaGib)))
	{
		var banned = GetCurrentBannedTeamChampions(GetPlayerTeam(GetOwner()));
		
		for (var c in banned)
		{
			if (c == champ)
			{
				Sound("UI::Error", true, 50, player);
				return false;
			}
		}
	}
	
	for (var c in GetBannedChampions())
	{
		if (c == champ)
		{
			Sound("UI::Error", true, 50, player);
			return false;
		}
	}
	
	ChampType = champ;
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

	if (FindObject(Find_Func("UseTeamExclusiveChampions")))
	{
		var banned = GetCurrentBannedTeamChampions(GetPlayerTeam(GetOwner()));
		for(var c in banned)
		{
			var champ_index = GetIndexOf(possible, c) + 101;
			GuiUpdate({BackgroundColor = 0x50ff0000}, choosemenu_id, champ_index, nil);
		}
	}
	
	for (var c in GetBannedChampions())
	{
		var champ_index = GetIndexOf(possible, c) + 101;
		GuiUpdate({BackgroundColor = 0x50ff0000}, choosemenu_id, champ_index, nil);
	}
}