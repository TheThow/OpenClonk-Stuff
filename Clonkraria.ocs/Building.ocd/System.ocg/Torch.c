#appendto Torch

public func ControlUse(object clonk)
{
	// Attach the torch if the clonk stands in front of tunnel material.
	if (GetMaterial() == Material("Tunnel") || FindObject(Find_AtPoint(), Find_Func("IsWallBuildingTile")))
	{
		// Do an attach animation. 
		clonk->DoKneel(); // For now kneel.
		// Attach the torch to the wall.
		AttachToWall();
		return true;
	}
	// Otherwise log a message about where one can attach torches.
	Message("$MsgTorchAttach$");
	return true;
}

// Attaches the torch to the wall.
public func AttachToWall(bool fixed)
{
	// Exit the torch and make it a non-collectible static back, also change its state.
	if (Contained()) Exit(0, 3);
	SetCategory(C4D_StaticBack);
	this.Collectible = false;
	state = TRCH_Attached;
	if (fixed)
		state = TRCH_Fixed;
	// Set plane so that it is in the background.
	this.Plane = 15;
	// Rotate the head of the torch a little into the screen.
	this.MeshTransformation = Trans_Rotate(-20, 1, 0, 0);
	// Add a burning effect if not already done.
	if (!GetEffect("IntBurning", this))
		AddEffect("IntBurning", this, 100, 1, this);
	return;
}
