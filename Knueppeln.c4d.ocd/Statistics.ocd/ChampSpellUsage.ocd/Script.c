/**
	Statistics: Spell Usage
	
	@author Maikel
*/


local stats;

public func Initialize()
{
	stats = {};
	return;
}

public func OnChampDoubleJump(object clonk, string direction)
{
	return OnChampSpellUsage(clonk, "DoubleJump");
}

public func OnChampBlock(object clonk)
{
	return OnChampSpellUsage(clonk, "Block");
}

public func OnChampSpellQ(object clonk)
{
	return OnChampSpellUsage(clonk, "SpellQ");
}

public func OnChampSpellE(object clonk)
{
	return OnChampSpellUsage(clonk, "SpellE");
}

public func OnChampSpellR(object clonk)
{
	return OnChampSpellUsage(clonk, "SpellR");
}

public func OnChampUseSword(object clonk)
{
	return OnChampSpellUsage(clonk, "Sword");
}

public func OnChampUseSpecialSword(object clonk)
{
	return OnChampSpellUsage(clonk, "SpecialSword");
}

public func OnChampSpellUsage(object clonk, string type)
{
	if (!clonk || GetPlayerType(clonk->GetOwner()) != C4PT_User)
		return;
	var champ_str = Format("%i", clonk.ChampType);
	if (!stats[champ_str])
		stats[champ_str] = {};
	if (!stats[champ_str][type])
		stats[champ_str][type] = 1;
	else
		stats[champ_str][type]++;
	return;
}

public func CollectStats()
{
	var champ_stats = FindObject(Find_ID(Statistics_ChampSpellUsage));
	if (champ_stats)
		return champ_stats->GetStats();
	return nil;
}

public func GetStats() { return stats; }
