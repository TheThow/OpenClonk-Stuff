#include AI

func AddAI(object clonk, sps)
{
	var fx = AI->AddAI(clonk);
	if (fx)
	{
		clonk.ExecuteAI = KnueppelnAI.Execute;
		fx.ai = KnueppelnAI;
		fx.ignore_allies = true;
		fx.spawns = sps;
	}
	return fx;
}

func Execute(proplist fx, int time)
{
	// For every Champ
	
	// Don't do anything when still in respawncontainer
	if (fx.Target->Contained()) return;
	
	// Avoid falling into Lava by doublejumping
	if (fx.Target->GetAction() == "Jump")
	{
		var end = fx.Target->SimFlight();
		
		if (Scenario->GetMaterial(end[0], end[1] -4) == Material("DuroLava"))
		{
			if (fx.Target->CanCast() && (!GetEffect("SpecialMeleeCD", fx.Target) || fx.Target.ChampType->CanCastSpecialMelee(fx.Target)))
			{
				ExecuteSpecialMeleeAttack(fx, RandomX(-10,10), -50);
			}
			else
			{
				ExecuteDoubleJump(fx);
			}
		}
	}
	
	// Attack with Melee Attack when target is near
	if (fx.target)
	{
		if (Distance(fx.Target->GetX(),fx.Target->GetY(),fx.target->GetX(),fx.target->GetY()) < 100)
		{
			ExecuteSpecialMeleeAttack(fx);
		}
	}
	
	if (fx.Target.ChampType == FireMan)
	{
		// Fireman is immune to fire. exclude fire from this
		if (fx.Target->FindObject(Find_Distance(30), Find_Func("IsReflectable"), Find_Not(Find_Owner(fx.Target->GetOwner())) ))
		{
			ExecuteBlock(fx);
		}
	}
	else
	{
		if (fx.Target->FindObject(Find_Distance(30), Find_Func("IsReflectable"), Find_Not(Find_Owner(fx.Target->GetOwner())) ))
		{
			ExecuteBlock(fx);
		}
	}
	
	//Specific behaviour for Champs
	if (fx.Target.ChampType == FireMan)
	{
		// For FireMan
		
		// Fire Q
		if (fx.target)
		{
			if (PathFree(fx.Target->GetX(),fx.Target->GetY(),fx.target->GetX(),fx.target->GetY()))
			{
				if (Random(10) < 8) ExecuteQSpell(fx);
				else ExecuteESpell(fx);
			}
			else
			{
				if (Distance(fx.Target->GetX(),fx.Target->GetY(),fx.target->GetX(),fx.target->GetY()) < fx.Target.ChampType.Special3Spell.SpellRange)
				{
					ExecuteRangedRSpell(fx);
				}
			}
		}
	}
	
	if (fx.Target.ChampType == ElectroMan)
	{
		// For ElectroMan
		
		// Fire Q
		if (fx.target)
		{
			if (PathFree(fx.Target->GetX(),fx.Target->GetY(),fx.target->GetX(),fx.target->GetY()))
			{
				ExecuteQSpell(fx);
			}
			else
			{
				if (Distance(fx.Target->GetX(),fx.Target->GetY(),fx.target->GetX(),fx.target->GetY()) < fx.Target.ChampType.Special3Spell.SpellRange)
				{
					ExecuteRangedRSpell(fx);
				}
			}
		}
	}
	
	
	
	return inherited(fx, time, ...);
}

func ExecuteQSpell(fx)
{
	//special_active[1] = !release;
		
	var a = GetAICursor(fx);
	//var x1 = a[0] - fx.Target->GetX();
	//var y1 = a[1] - fx.Target->GetY();
	
	fx.Target->LaunchSpecial1(a[0], a[1], false, false, fx.Target->CanCast() && fx.Target.ChampType->CanCastSpecial1(fx.Target));
}

func ExecuteRangedQSpell(fx)
{
	//special_active[1] = !release;
		
	var a = GetAICursor(fx);
	//var x1 = a[0] - fx.Target->GetX();
	//var y1 = a[1] - fx.Target->GetY();
	
	fx.Target->LaunchSpecial1(a[0], a[1], false, true, fx.Target->CanCast() && fx.Target.ChampType->CanCastSpecial1(fx.Target));
}

func ExecuteESpell(fx)
{
	//special_active[1] = !release;
		
	var a = GetAICursor(fx);
	//var x1 = a[0] - fx.Target->GetX();
	//var y1 = a[1] - fx.Target->GetY();
	
	fx.Target->LaunchSpecial2(a[0], a[1], false, false, fx.Target->CanCast() && fx.Target.ChampType->CanCastSpecial2(fx.Target));
}

func ExecuteRangedESpell(fx)
{
	//special_active[1] = !release;
		
	var a = GetAICursor(fx);
	//var x1 = a[0] - fx.Target->GetX();
	//var y1 = a[1] - fx.Target->GetY();
	
	fx.Target->LaunchSpecial2(a[0], a[1], false, true, fx.Target->CanCast() && fx.Target.ChampType->CanCastSpecial2(fx.Target));
}

func ExecuteRSpell(fx)
{
	//special_active[1] = !release;
		
	var a = GetAICursor(fx);
	//var x1 = a[0] - fx.Target->GetX();
	//var y1 = a[1] - fx.Target->GetY();
	
	fx.Target->LaunchSpecial3(a[0], a[1], false, false, fx.Target->CanCast() && fx.Target.ChampType->CanCastSpecial3(fx.Target));
}

func ExecuteRangedRSpell(fx)
{
	//special_active[1] = !release;
		
	var a = GetAICursor(fx);
	//var x1 = a[0] - fx.Target->GetX();
	//var y1 = a[1] - fx.Target->GetY();
	
	fx.Target->LaunchSpecial3(a[0], a[1], false, true, fx.Target->CanCast() && fx.Target.ChampType->CanCastSpecial3(fx.Target));
}

func ExecuteSpecialMeleeAttack(fx, atX, atY)
{
	var a;
	if (!atX && !atY)
		a = GetAICursor(fx);
	else
		a = [atX, atY];

	var flag = fx.Target.ChampType->SpecialMeleeAttack(fx.Target, a[0], a[1], false, false, fx.Target->CanCast() && fx.Target.ChampType->CanCastSpecialMelee(fx.Target), GetEffect("SpecialMeleeCD", fx.Target));
	
	if(flag)
		AddEffect("SpecialMeleeCD", fx.Target, 1, fx.Target.SPECIAL_MELEE_CD, fx.Target);
}

func ExecuteDoubleJump(fx)
{	
	if (GetEffect("IntControlJumpDouble", fx.Target))
	{
		if(fx.Target->GetMagicEnergy() >= fx.Target.JUMP_MANA)
		{
			RemoveEffect("IntControlJumpDouble", fx.Target);
			var ok = fx.Target->ControlUpDouble();
			if(ok)
			{
				fx.Target->JumpEffect(fx.Target, "Up");
				fx.Target->DoMagicEnergy(-fx.Target.JUMP_MANA);
			}
		}
		else
		{
			Sound("UI::Error", 0, 50, GetOwner());
		}
	}
	
	if(!GetEffect("JumpMCD", fx.Target))
	{
		AddEffect("IntControlJumpDouble", fx.Target, 1, fx.Target.TIMER);
		AddEffect("JumpMCD", fx.Target, 1, fx.Target.MOVEMENT_CD);
	}
}

func ExecuteBlock(fx)
{
	if(!GetEffect("BlockingCD", fx.Target))
	{
		AddEffect("Blocking", fx.Target, 1, 1, fx.Target, fx.Target->GetID());
		AddEffect("BlockingCD", fx.Target, 1, fx.Target.BLOCK_CD);
	}
}

func ExecuteIdle(fx)
{
	var inh = inherited(fx, ...);
	
	// Set random Spawnpoint to run to when idle
	if (!inh)
	{
		var rndspawn = fx.spawns[Random(GetLength(fx.spawns))];
		SetHome(fx.Target, rndspawn[0], rndspawn[1]);
	}
	
	return inh;
}

func FindTarget(fx)
{
	return inherited(fx, ...);
}

func GetAICursor(fx)
{
	if (fx.target) return [fx.target->GetX() - fx.Target->GetX(), fx.target->GetY() - fx.Target->GetY()];
	else return [0, 5]; // if AI has no target, aim above its head.
}
