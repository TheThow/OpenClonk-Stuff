/**

	Lazor Launcher
	
	@author: K-Pone

*/

local tox;
local toy;

local dely;

local SpellDamage;

local delayfx = new Effect
{
	Timer = func()
	{
		if (this.Time > Target.dely)
		{
			Target->FirenMaLazor();
		}
	}
};

func Initialize()
{
	
}

public func LazorTo(int to_x, int to_y, int delay, int damage)
{
	tox = to_x;
	toy = to_y;
	dely = delay;
	var dist = Distance(0, 0, tox, toy);
	if (dist == 0) dist = 1;
	SpellDamage = damage * 100 / dist;
	
	this->CreateEffect(delayfx,1,2);
}

public func HitObject(obj)
{
	if(obj->~CanBeHit() == false)
		return;
	obj->Fling(0, -2, nil, true);
	WeaponDamage(obj, SpellDamage);
}

func FirenMaLazor()
{
	var MaxLength = Distance(0, 0, tox, toy);
	
	var from_x = 0;
	var from_y = 0;
	
	var angle = Angle(0, 0, tox, toy);
	
	var r_bas = RandomX(0,120);
	var g_bas = RandomX(0,120);
	var b_bas = RandomX(0,120);
	
	for (var i = 0; i <= MaxLength; i += 5)
	{
		var posx = Sin(angle, i);
		var posy = Cos(angle, i) * -1;
		
		var lazorflash = 
		{
			Prototype = Particles_ElectroSpark2(),
			Size = PV_Linear(RandomX(8,13),0),
			Rotation = 0,
			R = r_bas + RandomX(0,30),
			G = g_bas + RandomX(0,30),
			B = b_bas + RandomX(0,30),
			Alpha = PV_Linear(100,20),
			OnCollision = PC_Bounce(500),
		};
		
		CreateParticle("Flash", posx, posy, Sin(angle + RandomX(-2, 2), RandomX(2,4)), Cos(angle + RandomX(-2,2), RandomX(2,4)) * -1, 40, lazorflash, 10);
		if (i % 20 == 0) CreateLight(posx, posy, 50, 1, nil, 0, 40);
	}
	var dummy = CreateObject(Dummy);
	dummy->Sound("Flash::spectralhit");
	dummy->RemoveObject();
	
	for (var obj in FindObjects(Find_OnLine(from_x, from_y, tox, toy), Find_Exclude(this.shooter), Find_Func("CanBeHit")))
	{
		this->HitObject(obj);
	}
	
	RemoveObject();
}