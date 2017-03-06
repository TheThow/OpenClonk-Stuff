/**
	Defense Tower
	Fire the typical champ projectiles automatically at enemies.
	
	@author Maikel
*/


// List of available projectiles.
local AvailableProjectiles = [TimeProjectile, EarthProjectile, SimpleArrow, Batarang, ElectroProjectile, IceProjectile, ExplosiveHook, PoisonDart, FireProjectile, FlashFlinger, FlashPrisma];
local current_projectile = 0;

public func Initialize()
{
	var firing_rate = 10;
	CreateEffect(FxFiringControl, 1, firing_rate, RandomElement(AvailableProjectiles));
	return;
}

public func SetProjectile(id projectile)
{
	var fx = GetEffect("FxFiringControl", this);
	if (fx)
		fx.projectile = projectile;
	return;
}


/*-- Interactions --*/

public func IsInteractable(object clonk) { return true; }

public func GetInteractionMetaInfo(object clonk)
{
	return { Description = Format("Cycle to projectile %s", AvailableProjectiles[current_projectile]->GetName()) };
}

public func Interact(object clonk)
{
	current_projectile++;
	if (current_projectile >= GetLength(AvailableProjectiles))
		current_projectile = 0;
	SetProjectile(AvailableProjectiles[current_projectile]);			
	return true;
}


/*-- Firing Effect --*/

local FxFiringControl = new Effect
{
	Construction = func(id proj)
	{
		this.projectile = proj;
		return FX_OK;
	},

	Timer = func(int time)
	{
		var owner = this.Target->GetOwner();
		var firing_at = GetFiringCoordinates();
		if (!firing_at)
			return FX_OK;
		
		FireProjectile(firing_at[0], firing_at[1]);
		return FX_OK;
	},
	
	GetFiringCoordinates = func()
	{
		var shot_speed = this.projectile.Speed;
		var straight_shot = this.projectile.IsStraight;
		var shot_range = this.projectile.Range;
		var shot_through_walls = false;
		var range_criteria = nil;
		if (shot_range)
			range_criteria = Find_Distance(shot_range);
		for (var target in this.Target->FindObjects(Find_Func("CanBeHit"), Find_Hostile(this.Target->GetOwner()), range_criteria, Sort_Distance()))
		{
			var x = this.Target->GetX(), y = this.Target->GetY();
			var tx = target->GetX(), ty = target->GetY() - 4;
			var d = Distance(x, y, tx, ty);
			// Correct for projected movement of target.
			var dt = d * 10 / shot_speed;
			tx += KnueppelnAI->GetTargetXDir(target, dt);
			ty += KnueppelnAI->GetTargetYDir(target, dt);
			if (!target->GetContact(-1))
				if (!target->GetCategory() & C4D_StaticBack)
					ty += GetGravity() * dt * dt / 200;	
			if (straight_shot)
			{
				if ((!PathFree(x, y, tx, ty) && !shot_through_walls) || (shot_range && Distance(x, y, tx, ty) > shot_range))
					continue;
				// Clear shot.
				return [tx - x, ty - y];			
			}
			else
			{
				var angle = KnueppelnAI->GetBallisticAngle(x, y, tx, ty, shot_speed, 160);
				if (angle == nil)
					return false;
				return [Sin(angle, 1000, 10), -Cos(angle, 1000, 10)];			
			}
		}		
		return nil;
	},
	
	FireProjectile = func(int x, int y)
	{
		var proj = this.Target->CreateObject(this.projectile);
		proj->SetController(this.Target->GetOwner());
		proj->Launch(this.Target, x, y);
		return;
	},
	
	Destruction = func()
	{
		return FX_OK;
	}
};


/*-- Properties --*/

local Name = "$Name$";
local Description = "$Description$";
local IsMinionTarget = true;