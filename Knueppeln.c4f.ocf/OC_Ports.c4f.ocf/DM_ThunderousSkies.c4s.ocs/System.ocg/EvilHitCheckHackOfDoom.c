global func FxHitCheckDoCheck(object target, proplist effect)
{
	var obj;
	// rather search in front of the projectile, since a hit might delete the effect,
	// and clonks can effectively hide in front of walls.
	var oldx = target->GetX();
	var oldy = target->GetY();
	var newx = target->GetX() + target->GetXDir() / 10;
	var newy = target->GetY() + target->GetYDir() / 10;
	var dist = Distance(oldx, oldy, newx, newy);
	
	var shooter = effect.shooter;
	var live = effect.live;
	
	if (live)
		shooter = target;
	
	if (dist <= Max(1, Max(Abs(target->GetXDir()), Abs(target->GetYDir()))) * 2)
	{
		// We search for objects along the line on which we moved since the last check
		// and sort by distance (closer first).
		for (obj in FindObjects(Find_OnLine(oldx, oldy, newx, newy),
								Find_NoContainer(),
								Find_Layer(target->GetObjectLayer()),
								Find_PathFree(target),
								Sort_Distance(oldx, oldy)))
		{
			// Excludes
			if (!obj) continue; // hit callback of one object might have removed other objects
			if(obj == target) continue;
			if(obj == shooter) continue;

			// Unlike in hazard, there is no NOFF rule (yet)
			// CheckEnemy
			//if(!CheckEnemy(obj,target)) continue;

			// IsProjectileTarget will be hit (defaults to true for OCF_Alive).
			if (obj->~IsProjectileTarget(target, shooter))
			{
				if(obj->GetID() == TargetBalloon)
				{
					obj->~OnProjectileHit(target);
				}
				
				target->~HitObject(obj);
				if (!target)
					return;
			}
		}
	}
	return;
}