/**

	Manaspawn
	
	@author: K-Pone

*/

#include EffectSpawn

local respawntime = 350; // spawntimer will be set to this value when buff is taken

local fxR = 80;
local fxG = 80;
local fxB = 255;

// Happens when the Spawn is collected. Can be overwritten via #include
func ReceiveSpawnBonus(object clonk)
{
	clonk->DoMagicEnergy(1000, true, 100);
	Sound("ManaGet", true);
	
	var particle_ring =
	{
		R = 100,
		G = 100,
		B = 255,
		BlitMode = GFX_BLIT_Additive,
		Size = PV_Linear(50,0),
		Alpha = PV_Linear(255,128),
		Attach = ATTACH_MoveRelative | ATTACH_Back,
	};
	
	clonk->CreateParticle("MagicRing", 0, 0, 0, 0, 35, particle_ring);
}