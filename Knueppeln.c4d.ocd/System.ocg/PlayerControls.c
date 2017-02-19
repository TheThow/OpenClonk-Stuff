static g_player_cursor_pos_ingame; // array of [x,y] pos arrays; indexed by player. last cursor pos as sent by CON_CursorPos

global func Control2Player(int plr, int ctrl, int x, int y, int strength, bool repeat, bool release)
{
	// cursor pos info - store in player values
	if (ctrl == CON_AimingCursor)
	{
		if (!g_player_cursor_pos_ingame)
			g_player_cursor_pos_ingame = [];
		g_player_cursor_pos_ingame[plr] = [x, y];
		return true;
	}
	
	return inherited(plr, ctrl, x, y, strength, repeat, release, ...);
}

/* return info of last sent CON_CursorPos packet for that player as [x, y] */
global func GetPlayerCursorPos(int plr, bool ingame)
{
	if (!ingame) return inherited(plr, ...);
	if (!g_player_cursor_pos_ingame) return nil;
	return g_player_cursor_pos_ingame[plr];
}
