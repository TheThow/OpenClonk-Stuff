#appendto Wipf

public func Initialize()
{
	AddEffect("XDir", this, 1, 1, this);
	return _inherited();
}

func FxXDirTimer()
{
	if(GetXDir() > 30)
		SetXDir(GetXDir() - Abs(GetXDir()/10));
	if(GetXDir() < -30)
		SetXDir(GetXDir() + Abs(GetXDir()/10));
	
	if(GetYDir() < - 20)
		SetYDir(GetYDir() + Abs(GetYDir()/10));
}

public func FxIntActivityTimer(object target, proplist effect, int time)
{
	if (GetYDir() == 0 && GetAction() != "Jump")
	{
		SetComDir(COMD_Stop);
		SetAction("Stand");
		SetXDir(0);
		return FX_OK;
	}
	return FX_OK;
}

public func FxIntActivityDamage()
{
	return 0;
}

public func CanBeHit() { return true; }

public func Incinerate() { return 0; }