global func BlastObject(int level, int caused_by)
{
	if (caused_by == nil)
		caused_by = GetController();

	if(this && GetPlayerTeam(this->GetOwner()) && GetOwner() != caused_by && GetPlayerTeam(this->GetOwner()) == GetPlayerTeam(caused_by) && FindObject(Find_ID(Rule_NoFriendlyFire)))
		return false;
	
	return _inherited(level, caused_by);
}