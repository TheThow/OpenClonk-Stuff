/**
	Triple Block
	Awarded when you block more than three projectile attacks with the same block.

	@author Maikel
*/


/*-- Medal Properties --*/

local Name = "$Name$";
local Description = "$Description$";

public func IsMedal() { return true; }
public func GetMedalIndex() { return 101; }
public func GetMedalReward() { return 25; }


/*-- Medal Scripts --*/

// Called when the round starts, technically when the rule is created.
public func OnRoundStart()
{
	AddEffect("IntMedalTripleBlock", nil, 100, 0, nil, Medal_TripleBlock);
	return;
}

// Called when the round is finished, i.e. when all players are elimanted
// or all goals are fulfilled.
public func OnRoundFinish()
{
	RemoveEffect("IntMedalTripleBlock", nil);
	return;
}

// Called when an attack has been blocked.
public func OnAttackBlock(object by_clonk, object blocked, effect fx)
{
	// Notify the effect that keeps track of the triple block medal.
	var fx_medal = GetEffect("IntMedalTripleBlock", nil);
	if (fx_medal)
		EffectCall(nil, fx_medal, "OnAttackBlock", by_clonk, blocked, fx);
	return;
}


/*-- Medal Effects --*/

// Medal triple block effect keeps track of performed blocks.
public func FxIntMedalTripleBlockStart(object target, proplist effect, int temporary)
{
	if (temporary)
		return FX_OK;
	return FX_OK;
}

// Effect callback made when an attack has been blocked.
public func FxIntMedalTripleBlockOnAttackBlock(object target, proplist effect, object by_clonk, object blocked, effect fx)
{
	// Initialize block list for the block effect and add the blocked object.
	if (!fx.medal_blocked_list)
		fx.medal_blocked_list = [];
	PushBack(fx.medal_blocked_list, blocked);
	RemoveDuplicates(fx.medal_blocked_list);
	// Award the medal if three different objects have been blocked.
	if (GetLength(fx.medal_blocked_list) >= 3)
	{
		Rule_Medals->~AwardMedal(Medal_TripleBlock, by_clonk->GetOwner());	
		fx.medal_blocked_list = nil;
	}
	return FX_OK;
}

// Medal triple block effect keeps track of performed blocks.
public func FxIntMedalTripleBlockStop(object target, proplist effect, int reason, bool temporary)
{
	if (temporary)
		return FX_OK;
	return FX_OK;
}
