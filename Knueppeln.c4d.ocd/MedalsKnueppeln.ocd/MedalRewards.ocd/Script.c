/**
	Medal Rewards
	An alternative reward for achieving a medal.

	@author Maikel	
*/


protected func Initialize()
{
	// Only one object of this type.
	if (ObjectCount(Find_ID(Env_MedalRewards)) > 1)
		return RemoveObject();
	return;
}

// Callback by the medal rule when a medal has been awarded.
public func OnMedalAwarded(id medal, int plr)
{
	var mana_bonus = medal->~GetMedalReward();
	if (!mana_bonus)
		return;
	var cursor = GetCursor(plr);
	if (!cursor)
		return;
	cursor->DoMagicEnergy(mana_bonus, true);
	return;
}