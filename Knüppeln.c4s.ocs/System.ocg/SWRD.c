#appendto Sword

func ControlThrow()
{
	return true;
}

func SwordDamage(int shield)
{
	return ((100-shield)*15*1000 / 100);
}