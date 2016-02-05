#appendto Clonk

func FxIntTurnTimer(pTarget, effect, iTime)
{
	// Check wether the clonk wants to turn (Not when he wants to stop)
	var iRot = effect.rot;
	if( (effect.dir != GetDirection() || this->~IsAiming()) || effect.turn_type != lAnim.turnType) 
	{
		effect.dir = GetDirection();
		if(effect.dir == COMD_Right)
		{
			if(lAnim.turnType == 0)
				iRot = 180-25;
			if(lAnim.turnType == 1)
				iRot = 180;
		}
		else
		{
			if(lAnim.turnType == 0)
				iRot = 25;
			if(lAnim.turnType == 1)
				iRot = 0;
		}
		// Save new ComDir
		effect.dir = GetDirection();
		effect.turn_type = lAnim.turnType;
		// Notify effects
//		ResetAnimationEffects();
	}
	if(iRot != effect.curr_rot)
	{
		effect.curr_rot += BoundBy(iRot-effect.curr_rot, -18, 18);
		SetMeshTransformation(Trans_Rotate(effect.curr_rot, 0, 1, 0), 0);
	}
	effect.rot = iRot;
	return;
}
