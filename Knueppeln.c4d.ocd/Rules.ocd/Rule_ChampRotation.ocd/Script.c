/**
	Rule_RandomChamp
	

	@author 
*/

local champs;
local plr_index;

func Initialize()
{
	champs = ShuffleArray(GetChampionsWithoutBanned());
	plr_index = CreateArray(GetPlayerCount());
}

func GetChamp(int plr)
{
	var ret = champs[plr_index[plr]];
	plr_index[plr]++;
	
	if(plr_index[plr] == GetLength(champs))
	{
		plr_index[plr] = 0;
	}
	
	return ret;
}