public func SaveScenarioObject() { return false; }

local base;

local target;
local length;
local start_y;

local MovingEffect = new Effect
{
	Timer = func (int time)
	{
		var phase = time;
		this.Target->SetPosition(this.Target->GetX(), this.Target.start_y + Sin(phase, this.Target.length));
		
		if (phase >= 90)
		{
			this.Target->OnMoveEnd();
			return FX_Execute_Kill;
		}
	}
};

local RetractMarker = new Effect
{
	Start = func() { this.Target.is_being_moved_by_conveyor = true; },
	Stop = func() { this.Target.is_being_moved_by_conveyor = false; },
};

local RetractEffect = new Effect
{
	SetPayload = func(payload)
	{
		if (!payload) return;
		this.payload = payload;
		this.payload_fx = payload->CreateEffect(Tile_ConveyorArmHook.RetractMarker, 1, 0);
		payload->SetRDir();
	},
	
	Stop = func()
	{
		if (this.payload_fx)
			RemoveEffect(nil, nil, this.payload_fx);
	},
	
	Timer = func (int time)
	{
		var this_y = this.Target->GetY();
		var target_y = this.Target.base->GetY();  
		if (this_y != target_y)
			this.Target->SetPosition(this.Target.base->GetX(), this_y + BoundBy(target_y - this_y, -2, 2));
		else
		if (!this.payload || this.payload->Contained())
		{
			this.Target.base->RemoveObject();
			return FX_Execute_Kill;
		}
		else if (!this.is_moving)
		{
			this.Target.base->StartMoving(this.payload);
			this.is_moving = true;
		}
		
		if (this.payload)
		{
			this.payload->SetPosition(this.Target->GetX(), this.Target->GetY());
			this.payload->SetSpeed(0, 0);
		}
	}
};

public func MoveToGrab(object target)
{
	this.target = target;
	start_y = GetY();
	length = target->GetY() - start_y;
	this.Plane = target.Plane + 1;
	CreateEffect(MovingEffect, 1, 1);
	Sound("Structures::Elevator::Start", {pitch = 200});
}

public func OnMoveEnd()
{
	Sound("Structures::Elevator::Stop", {pitch = 200});
	Sound("Hits::Materials::Metal::DullMetalHit*", {pitch = 150});
	var still_target = FindObject(Find_AtPoint(), Find_InArray([target]));
		
	var fx = CreateEffect(RetractEffect, 1, 1);
	fx->SetPayload(still_target);
}