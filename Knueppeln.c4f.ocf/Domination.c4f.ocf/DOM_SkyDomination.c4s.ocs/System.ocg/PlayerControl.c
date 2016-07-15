/*--
		QuickBuy
--*/


global func PlayerControl(int plr, int ctrl)
{
	if (ctrl == CON_ToggleShop)
	{
		var buy_menu = FindObject(Find_ID(GUI_BuyMenu), Find_Owner(plr));
		if (!buy_menu) return false;
		return buy_menu->ToggleVisibility(plr);
	}
	return _inherited(plr, ctrl, ...);
}