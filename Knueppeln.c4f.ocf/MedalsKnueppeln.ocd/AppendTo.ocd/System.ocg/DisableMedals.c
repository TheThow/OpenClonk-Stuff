// This disables medals which are shipped with the medal rule so that these id's can be used for new medals.
// Still it is probably better to use unique id's as long as these are available. This also removes medals
// from the medal menu, such that medals irrelevant for this round are not shown to the players.

#appendto Medal_Miner
#appendto Medal_Pyromania
#appendto Medal_Demolition
#appendto Medal_ChainReaction
#appendto Medal_Rocketeer
#appendto Medal_Construction
#appendto Medal_Plumber

// Overloading the IsMedal function to return false suffices.
public func IsMedal() { return false; }
