
global func SpawnProtection(int dur)
{
	if(!dur)
		dur = 120;
	
	var fx = AddEffect("SpawnProtection", this, 20, 1);
	fx.dur = dur;
}

global func FxSpawnProtectionStart(object target, proplist effect, int temporary)
{
	if(temporary)
		return;

	target->SetObjectLayer(target);
	target->SetClrModulation(RGBa(255,255,255, 100));
	effect.dummy = CreateObject(Dummy, target->GetX(), target->GetY(), target->GetOwner());
	effect.dummy.Visibility = VIS_All;
	effect.dummy->SetAction("HangOnto", target);
	var props =
		{
			R = 255,
			G = 255,
			B = 255,
			Alpha = 100,
			Size = 40,
			BlitMode = GFX_BLIT_Additive,
			Rotation = PV_Step(10, 0, 1),
			Attach = ATTACH_Back | ATTACH_MoveRelative
			
	};
	effect.dummy->CreateParticle("Shockwave", 0, 0, 0, 0, 0, props);
}

global func FxSpawnProtectionTimer(object target, proplist effect, int time)
{
	if(time > effect.dur)
		return -1;
}

global func FxSpawnProtectionDamage()
{
	return 0;
}

global func FxSpawnProtectionStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;
		
	if(effect.dummy)
		effect.dummy->RemoveObject();
		
	if(!target)
		return;
		
	target->SetObjectLayer(nil);
	target->SetClrModulation(RGBa(255,255,255, 255));
}