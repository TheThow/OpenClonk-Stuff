/**
	Knueppeln AI
	Makes men stronger.

	@author K-Pone, Maikel
*/


#include AI


// AI Settings.
local MaxAggroDistance = 800; // Lose sight to target if it is this far away (unless we're ranged - then always guard the range rect).
local KnueppelnLoggingOn = false; // Whether or not debug logging is turned on.


public func Execute(effect fx, int time)
{
	// Don't do anything when still in respawn container.
	if (fx.Target->Contained())
		return;
		
	// Find a target to attack (also change targets from time to time).
	if (!fx.target || fx.Target->ObjectDistance(fx.target) > RandomX(150, 800))
		fx.target = this->FindTarget(fx);

	// Show energy/magic energy.
	if (fx.control.KnueppelnLoggingOn)
		fx.Target->Message("@<c aa0000>%d</c>/<c 0000aa>%d</c>", fx.Target->GetEnergy(), fx.Target->GetMagicEnergy());		
	
	// Try to evade danger like lava.
	if (this->ExecuteEvadeDanger(fx))
		return;
	
	// First try to block danger.
	if (this->ExecuteBlock(fx))
		return;
	
	// Attack with melee attack when target is near.
	if (fx.target && fx.Target->ObjectDistance(fx.target) < 100)
	{
		if (this->ExecuteSpecialMeleeAttack(fx))
			return;
	}
	
	// Simple strategy for now: try stronger spells which require more magic energy first.

	// Try special spell 3 (R).
	if (!Random(4))
		if (this->ExecuteRSpell(fx))
			return;
			
	// Try special spell 2 (E).
	if (!Random(3))
		if (this->ExecuteESpell(fx))
			return;

	// Try special spell 1 (Q).
	if (Random(3))
		if (this->ExecuteQSpell(fx))
			return;
	
	// Execute special champ movement to reach the target.
	if (this->ExecuteChampMovement(fx))
		return;		
		
	// Do normal AI behavior.
	return inherited(fx, time, ...);
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
