/**
	Minion AI
	Just minions.

	@author K-Pone
*/


#include AI


// AI Settings.
local MaxAggroDistance = 800; // Lose sight to target if it is this far away (unless we're ranged - then always guard the range rect).
local KnueppelnLoggingOn = false; // Whether or not debug logging is turned on.


public func Execute(effect fx, int time)
{
	if (!(fx.AssignedLane))
		fx.AssignedLane = fx.Target.Lane;
	
	// Don't do anything when still in respawn container.
	if (fx.Target->Contained())
		return;

	// Show energy/magic energy.
	if (fx.control.KnueppelnLoggingOn)
		fx.Target->Message("@<c aa0000>%d</c>/<c 0000aa>%d</c>", fx.Target->GetEnergy(), fx.Target->GetMagicEnergy());		

	// Movement on Lanes
	// if (fx.AssignedLane == "Top")
	// {
		
	// }
	
	if (fx.AssignedLane == "Bot")
	{
		// Bot needs to drop through a bridge
		if (fx.Target->GetOwner() == g_plrid_minions_left)
		{
			if (fx.Target->GetX() > 500 && fx.Target->GetX() < 570)
			{
				fx.Target->HalfVehicleFadeJumpStart();
			}
			if (fx.Target->GetX() > 570)
			{
				fx.Target->HalfVehicleFadeJumpStop();
			}
		}
		
		if (fx.Target->GetOwner() == g_plrid_minions_right)
		{
			if (fx.Target->GetX() < 5900 && fx.Target->GetX() > 5830)
			{
				fx.Target->HalfVehicleFadeJumpStart();
			}
			if (fx.Target->GetX() < 5830)
			{
				fx.Target->HalfVehicleFadeJumpStop();
			}
		}
	}
	
	//if (fx.Target->FindContents(Bow))
	//{
	//	SetHome(fx.Target, fx.target->GetX(), fx.target->GetY());
	//}
	
	// Check for near targets
	var near = fx.Target->FindObject(Find_OCF(OCF_Alive), Find_Distance(100), Find_Hostile(fx.Target->GetOwner()), Sort_Distance());
	if (near)
	{
		fx.target = near;
	}



		
	// Do normal AI behavior.
	return inherited(fx, time, ...);
}

public func ExecuteEvade(effect fx, int threat_dx, int threat_dy)
{
	return false;
}

public func ExecuteProtection(effect fx)
{
	return false;
}

public func FindTarget(effect fx)
{
	var target = _inherited(fx, ...);
	
	//var target = GameCall("GiveRandomAttackTarget", fx.Target);
	
	if (!target || ObjectDistance(target, fx.Target) > 150)
	{
		if (fx.is_siege)
			target = GetRandomSiegeTarget(fx.Target);
		else		
			target = GetRandomAttackTarget(fx.Target);
	}
	
	return target;
}

/*-- Movement --*/

public func ExecuteEvadeDanger(effect fx)
{
	// Avoid falling into Lava by double jumping.
	if (fx.Target->GetAction() == "Jump" || fx.Target->GetAction() == "Tumble")
	{
		var end = fx.Target->SimFlight();
		
		if (Global->GetMaterial(end[0], end[1] - 4) == Material("DuroLava"))
		{
			if (this->ExecuteDoubleJump(fx, "Up"))
				return true;
		}
	}
	return false;
}

public func ExecuteDoubleJump(effect fx, string direction)
{
	if (!fx.Target->IsJumping())
		return false;
	if (fx.Target->GetMagicEnergy() < fx.Target.JUMP_MANA)
		return false;
	
	if (direction == "Up")
	{
		fx.Target->ControlUpDouble();
	}
	if (direction == "Dow ")
	{
		fx.Target->ControlDownDouble();
	}
	if (direction == "Left")
	{
		fx.Target->ControlLeftDouble();
	}
	if (direction == "Right")
	{
		fx.Target->ControlRightDouble();
	}
	this->LogKN(fx, Format("Executed double jump %s", direction));
	return true;
}

public func ExecuteChampMovement(effect fx)
{
	if (!fx.target)
		return false;
	
	// Try to get to the same level as the target or even above.
	if (fx.target->GetY() < fx.Target->GetY() - 30)
	{
		if (this->ExecuteJump(fx))
			return true;
	}
	
	if (fx.Target->IsJumping() && !GetEffect("MovementMCD", fx.Target))
	{
		var angle = Angle(fx.Target->GetX(), fx.Target->GetY(), fx.target->GetX(), fx.target->GetY());
		if (Inside(angle, -45, 45))
			if (this->ExecuteDoubleJump(fx, "Up"))
			{
				AddEffect("MovementMCD", fx.Target, 1, fx.Target.MOVEMENT_CD);
				return true;
			}
		if (Inside(angle, 45, 135))
			if (this->ExecuteDoubleJump(fx, "Right"))
			{
				AddEffect("MovementMCD", fx.Target, 1, fx.Target.MOVEMENT_CD);
				return true;
			}
		if (Inside(angle, -135, -45))
			if (this->ExecuteDoubleJump(fx, "Left"))
			{
				AddEffect("MovementMCD", fx.Target, 1, fx.Target.MOVEMENT_CD);
				return true;
			}
	}
	return false;
}


/*-- Special Melee Attack (F) --*/

public func ExecuteSpecialMeleeAttack(effect fx, int dx, int dy)
{
	if (!dx || !dy)
	{
		if (!fx.target)
			return false;
		dx = fx.target->GetX() - fx.Target->GetX();
		dy = fx.target->GetY() - fx.Target->GetY();
	}

	var flag = fx.Target.ChampType->SpecialMeleeAttack(fx.Target, dx, dy, false, false, fx.Target->CanCast() && fx.Target.ChampType->CanCastSpecialMelee(fx.Target), GetEffect("SpecialMeleeCD", fx.Target));
	if (flag)
	{
		AddEffect("SpecialMeleeCD", fx.Target, 1, fx.Target.SPECIAL_MELEE_CD, fx.Target);
		this->LogKN(fx, Format("Executed special melee attack to (%d, %d).", dx, dy));
		return true;
	}
	return false;
}


/*-- Special 1 Spell (Q) --*/

public func ExecuteQSpell(effect fx)
{
	if (!fx.target)
		return false;
	
	// Get champ type and target info.
	var type = fx.Target.ChampType;
	var x = fx.Target->GetX(), y = fx.Target->GetY();
	var tx = fx.target->GetX(), ty = fx.target->GetY() - 4;
	var d = Distance(x, y, tx, ty);

	// Perform shooting spells.
	if (type->~IsSpecial1Shot())
	{
		// Determine shot speed.
		var shot_speed = type->~IsSpecial1ShotSpeed();
		var dx, dy;

		// Correct for projected movement of target.
		var dt = d * 10 / shot_speed;
		tx += this->GetTargetXDir(fx.target, dt);
		ty += this->GetTargetYDir(fx.target, dt);
		if (!fx.target->GetContact(-1))
			if (!fx.target->GetCategory() & C4D_StaticBack)
				ty += GetGravity() * dt * dt / 200;	
		
		// Perform straight shot.
		if (type->~IsSpecial1ShotStraight())
		{
			var range = type->~IsSpecial1ShotRange();
			var through_walls = type->~IsSpecial1ShotThroughWalls();
			if ((!PathFree(x, y, tx, ty) && !through_walls) || (range && Distance(x, y, tx, ty) > range))
				return false;
			dx = tx - x;
			dy = ty - y;
		
		}
		// Perform ballistic shot.
		else
		{
			// Ballistic angle takes into account projectile speed and projected movement of target.
			var angle = this->GetBallisticAngle(x, y, tx, ty, shot_speed, 180);
			if (angle == nil)
				return false;
			dx = Sin(angle, 1000, 10);
			dy = -Cos(angle, 1000, 10);
		}
		if (fx.Target->LaunchSpecial1(dx, dy, false, false, fx.Target->CanCast() && type->CanCastSpecial1(fx.Target)))
		{
			this->LogKN(fx, "Execute special 1 spell (Q).");
			return true;
		}
		return false;
	}
	
	// Try to execute spell in champ.
	if (type->~ExecuteAISpecial1Spell(fx))
	{
		this->LogKN(fx, "Execute special 1 spell (Q).");
		return true;
	}
	
	// Spell type not yet implemented.
	this->LogKN(fx, Format("Q spell for %i not yet implemented.", type));
	return false;
}


/*-- Special 2 Spell (E) --*/

public func ExecuteESpell(effect fx)
{
	if (!fx.target)
		return false;

	// Get champ type and target info.
	var type = fx.Target.ChampType;
	var x = fx.Target->GetX(), y = fx.Target->GetY();
	var tx = fx.target->GetX(), ty = fx.target->GetY() - 4;
	var d = Distance(x, y, tx, ty);

	// Perform shooting spells.
	if (type->~IsSpecial2Shot())
	{
		// Determine shot speed.
		var shot_speed = type->~IsSpecial2ShotSpeed();
		var dx, dy;

		// Correct for projected movement of target.
		var dt = d * 10 / shot_speed;
		tx += this->GetTargetXDir(fx.target, dt);
		ty += this->GetTargetYDir(fx.target, dt);
		if (!fx.target->GetContact(-1))
			if (!fx.target->GetCategory() & C4D_StaticBack)
				ty += GetGravity() * dt * dt / 200;	
		
		// Perform straight shot.
		if (type->~IsSpecial2ShotStraight())
		{
			var range = type->~IsSpecial2ShotRange();
			var through_walls = type->~IsSpecial2ShotThroughWalls();
			if ((!PathFree(x, y, tx, ty) && !through_walls) || (range && d > range))
				return false;
			dx = tx - x;
			dy = ty - y;
		
		}
		// Perform ballistic shot.
		else
		{
			// Ballistic angle takes into account projectile speed and projected movement of target.
			var angle = this->GetBallisticAngle(x, y, tx, ty, shot_speed, 180);
			if (angle == nil)
				return false;
			dx = Sin(angle, 1000, 10);
			dy = -Cos(angle, 1000, 10);
		}
		if (fx.Target->LaunchSpecial2(dx, dy, false, false, fx.Target->CanCast() && type->CanCastSpecial2(fx.Target)))
		{
			this->LogKN(fx, "Execute special 2 spell (E).");
			return true;
		}
		return false;
	}
	
	// Try to execute spell in champ.
	if (type->~ExecuteAISpecial2Spell(fx))
	{
		this->LogKN(fx, "Execute special 2 spell (E).");
		return true;
	}	

	// Spell type not yet implemented.
	this->LogKN(fx, Format("E spell for %i not yet implemented.", type));
	return false;
}


/*-- Special 3 Spell (R) --*/

public func ExecuteRSpell(effect fx)
{
	if (!fx.target)
		return false;
		
	// Get champ type and target info.
	var type = fx.Target.ChampType;
	var x = fx.Target->GetX(), y = fx.Target->GetY();
	var tx = fx.target->GetX(), ty = fx.target->GetY() - 4;
	var d = Distance(x, y, tx, ty);

	// Perform shooting spells.
	if (type->~IsSpecial3Shot())
	{
		// Determine shot speed.
		var shot_speed = type->~IsSpecial3ShotSpeed();
		var dx, dy;

		// Correct for projected movement of target.
		var dt = d * 10 / shot_speed;
		tx += this->GetTargetXDir(fx.target, dt);
		ty += this->GetTargetYDir(fx.target, dt);
		if (!fx.target->GetContact(-1))
			if (!fx.target->GetCategory() & C4D_StaticBack)
				ty += GetGravity() * dt * dt / 200;	
		
		// Perform straight shot.
		if (type->~IsSpecial3ShotStraight())
		{
			var range = type->~IsSpecial3ShotRange();
			var through_walls = type->~IsSpecial3ShotThroughWalls();
			if ((!PathFree(x, y, tx, ty) && !through_walls) || (range && d > range))
				return false;
			dx = tx - x;
			dy = ty - y;
		
		}
		// Perform ballistic shot.
		else
		{
			// Ballistic angle takes into account projectile speed and projected movement of target.
			var angle = this->GetBallisticAngle(x, y, tx, ty, shot_speed, 180);
			if (angle == nil)
				return false;
			dx = Sin(angle, 1000, 10);
			dy = -Cos(angle, 1000, 10);
		}
		if (fx.Target->LaunchSpecial3(dx, dy, false, false, fx.Target->CanCast() && type->CanCastSpecial3(fx.Target)))
		{
			this->LogKN(fx, "Execute special 3 spell (R).");
			return true;
		}
		return false;
	}
	
	// Try to execute spell in champ.
	if (type->~ExecuteAISpecial3Spell(fx))
	{
		this->LogKN(fx, "Execute special 3 spell (R).");
		return true;
	}	

	// Spell type not yet implemented.
	this->LogKN(fx, Format("R spell for %i not yet implemented.", type));
	return false;
}


/*-- Blocking Spell (Space) --*/

public func ExecuteBlock(effect fx)
{
	var danger = fx.Target->FindObject(Find_Distance(fx.Target->GetBlockingRange() + 5), Find_Func("IsReflectable", fx.Target), Find_Not(Find_Owner(fx.Target->GetOwner())));
	if (!danger)
		return false;
	// Still waiting for cool down?	
	if (GetEffect("BlockingCD", fx.Target))
		return false;
	// Perform the block.
	AddEffect("Blocking", fx.Target, 1, 1, fx.Target, fx.Target->GetID());
	AddEffect("BlockingCD", fx.Target, 1, fx.Target.BLOCK_CD);
	this->LogKN(fx, "Executed a block.");
	return true;
}


/*-- AI Overloading --*/

public func ExecuteIdle(effect fx)
{
	// Just stay at current position and wait for enemies.
	return true;
}

// Checks whether the AI has a weapon for the target.
public func HasWeaponForTarget(effect fx, object target)
{
	if (target && target->~CanBeHit())
		return true;
	return _inherited(fx, target, ...);
}


/*-- Aiming Cursor --*/

// Called by spell charging effects.
public func GetAICursor(fx)
{
	if (fx.target)
		return [fx.target->GetX() - fx.Target->GetX(), fx.target->GetY() - fx.Target->GetY()];
	 // If the AI has no target, aim above its head.	
	return [0, 5];
}


/*-- Debug Logging --*/

public func LogKN(effect fx, string message)
{
	if (fx.control.KnueppelnLoggingOn)
		Log("[%d]%v(%i): %s", FrameCounter(), fx.Target, fx.Target.ChampType, message);
	return;
}
