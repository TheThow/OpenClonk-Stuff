/**

	Manaspawn
	
	@author: K-Pone

*/

local spawntimer = 0; // Countdown until the buff respawns
local respawntime = 350; // spawntimer will be set to this value when buff is taken
local ringtimer = 0; // counter for the particles around the spawn

// Effect colors
local fxR = 80;
local fxG = 80;
local fxB = 255;

func Initialize()
{
	AddEffect("Effectspawn", this, 1, 2, this);
}

private func FxEffectspawnTimer(object target, proplist effect, int time)
{
	if (spawntimer > 0)
	{
		// Spawn not respawned yet
		spawntimer--;
	}
	else
	{
		// Spawn has spawned Bonus object
		var receiver = FindObject(Find_Distance(20), Find_OCF(OCF_CrewMember));
		if (receiver)
		{
			ReceiveSpawnBonus(receiver);
			spawntimer = respawntime;
		}
		
		var particle_orb = 
		{
			R = PV_Random(50,150),
			G = PV_Random(50,150),
			B = 255,
			Alpha = PV_Linear(200,0),
			BlitMode = GFX_BLIT_Additive,
			Rotation = PV_Linear(PV_Random(0,360), PV_Random(0,360)),
			Size = PV_Linear(PV_Random(10,13),1)
		};
		CreateParticle("MagicSpark", RandomX(-2,2), RandomX(-2,2), RandomX(-2,2), RandomX(-2,2), 15, particle_orb, 3);
	}
	
	// Visual part of the spawn
	DoParticles();
}

func DoParticles()
{
	ringtimer += 15;
	if (ringtimer >= 360) ringtimer = 0;
	
	var particle;
	if (spawntimer > 0)
	{
		particle =
		{
			R = 140,
			G = 80,
			B = 80,
			Alpha = PV_Linear(200,0)
		};
	}
	else
	{
		particle =
		{
			R = 80,
			G = 80,
			B = 255,
			Alpha = PV_Linear(200,0)
		};
	}
	
	
	// Particles flying around the Spawn
	CreateParticle("Flash", Sin(ringtimer,     22), Cos(ringtimer,     10), 0, 0, 20, particle);
	CreateParticle("Flash", Sin(ringtimer+180, 22), Cos(ringtimer+180, 10), 0, 0, 20, particle);
	CreateParticle("Flash", Sin(ringtimer+90,  10), Cos(ringtimer+90,  22), 0, 0, 20, particle);
	CreateParticle("Flash", Sin(ringtimer+270, 10), Cos(ringtimer+270, 22), 0, 0, 20, particle);
}

// Happens when the Spawn is collected. Can be overwritten via #include
func ReceiveSpawnBonus(object clonk)
{
	
}