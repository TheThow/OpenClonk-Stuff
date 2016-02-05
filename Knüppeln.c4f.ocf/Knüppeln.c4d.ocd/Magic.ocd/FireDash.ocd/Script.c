/**
	FireNado
	

	@author 
*/

local Name = "$Name$";
local Description = "$Description$";

local ManaCost = 50;
local SpellRange = 180;

local Size1 = 25;
local Size2 = 40;
local SpellDamage1 = 20;
local SpellDamage2 = 40;

local Speed = 30;
local Durr = 70;
local Charge_durr = 20;

local angle_prec = 10;

func Initialize()
{
	SetAction("Travel");
	SetClrModulation(RGBa(0,0,0,0));
}

func Launch(object clonk, int x, int y)
{
	var marker = CreateObject(Icon_Cancel, clonk->GetX() + x - GetX(), clonk->GetY() + y - GetY() + 16, GetOwner());
	marker.Visibility = VIS_Owner;
	marker->SetClrModulation(RGBa(255,255,255,100));
	
	var params = {
		angle = Angle(0,0,x,y, angle_prec),
		clonk = clonk,
		x = x,
		y = y,
		marker = marker
	};
	clonk->Charge(this, "ChargeStop", Charge_durr, params);
	

}

func ChargeEffect(proplist params)
{
	var clonk = params.clonk;

	for(var i = 0; i < 360; i+= RandomX(3,15))
	{
		if(!Random(10))
		{
			var props =
			{
				Prototype = Particles_FireTrail(),
				Size = PV_Linear(10,0),
				BlitMode = GFX_BLIT_Additive,
				OnCollision=nil,
				CollisionVertex=nil
			};
			
			var xdir = Sin(i + 180, RandomX(15, 30));
			var ydir = -Cos(i + 180, RandomX(15, 30));
			
			CreateParticle("Fire", clonk->GetX() - GetX() + Sin(i, 30), clonk->GetY() - GetY() - Cos(i, 30), PV_Random(xdir -2, xdir + 2), PV_Random(ydir - 2 , ydir + 2), 20, props, 2);
		}
	}
}

func ChargeStop(proplist params)
{

	var eff = AddEffect("FireDash", params.clonk, 20, 1 ,nil, GetID());
	eff.angle = params.angle;
	eff.dist = Distance(0,0, params.x, params.y);
	eff.SpellDamage1 = SpellDamage1;
	eff.SpellDamage2 = SpellDamage2;
	eff.Size1 = Size1;
	eff.Size2 = Size2;
	eff.startx = params.clonk->GetX();
	eff.starty = params.clonk->GetY();
	eff.tx = params.x;
	eff.ty = params.y;
	eff.marker = params.marker;
	eff.clonk = params.clonk;
	eff.angle_prec = angle_prec;
	
	params.clonk->SetAction("Float");
	//params.clonk->MakeHitable(false);
	params.clonk->SetObjectLayer(params.clonk);
	
	Sound("Fire::Fireball", false, 100);
	RemoveObject();
}


func FxFireDashTimer(object target, proplist effect, int time)
{
	var a = effect.angle;
		
	var x = target->GetX();
	var y = target->GetY();
	
	target->SetPosition(target->GetX() + Sin(a, 6, effect.angle_prec), target->GetY() + -Cos(a, 6, effect.angle_prec));

	for(var o in FindObjects(Find_Distance(effect.Size1, x, y), Find_Func("CanBeHit")))
	{
		if(o->GetOwner() == target->GetOwner())
			continue;
		if(!GetEffect("DashCD", o))
		{
			o->Fling(0, -5);
			AddEffect("DashCD", o, 20, 10);
			o->AddFireHitEffect();
			WeaponDamage(o, effect.SpellDamage1);
		}
		
	}
	
	var chaoticspark =
	{
		Size = PV_Linear(1, 0),
		ForceX = PV_KeyFrames(10, 0, PV_Random(-6, 6), 333, PV_Random(-6, -6), 666, PV_Random(6, 6), 1000, PV_Random(-6, 6)),
		ForceY = PV_KeyFrames(10, 0, PV_Random(-8, 5), 333, PV_Random(-8, 5), 666, PV_Random(-10, 10), 1000, PV_Random(-10, 15)),
		Stretch = PV_Speed(1000, 500),
		Rotation = PV_Direction(),
		CollisionVertex = 0,
		OnCollision = PC_Die(),
		R = 255,
		G = PV_Linear(255,100),
		B = PV_Random(0, 100),
		DampingX=950,
		DampingY=950,
		Alpha = PV_Random(100,180),
		BlitMode = GFX_BLIT_Additive
	};
	CreateParticle("Magic", x + RandomX(-5, 5), y + RandomX(-10, 10), RandomX(25, -25) + target->GetXDir(), RandomX(-25, 12) + target->GetYDir(), 50, chaoticspark, 4);


	var firetrailparticles =
	{
		Prototype = Particles_FireTrail(),
		Size = PV_Linear(effect.Size1,0),
		BlitMode = GFX_BLIT_Additive,
		OnCollision=nil,
	};
	CreateParticle("Fire", x, y, PV_Random(-7,7), PV_Random(-7,7), 20, firetrailparticles, 3);
	
	var dist = Distance(x, y, effect.startx + effect.tx, effect.starty + effect.ty);
	if(dist < 10 || (dist > effect.dist + 1 && time > 10))
	{
		return -1;
	}
	else
	{
		//Log("%d %d", dist, effect.dist);
		effect.dist = dist;
	}
	
	return 0;
}

func ChargeInterrupted(params)
{
	if(params.marker)
		params.marker->RemoveObject();
	RemoveObject();
}

func FxFireDashStop(object target, proplist effect, int reason, bool temporary)
{
	if(temporary)
		return;
	
	target->SetAction("Jump");
	ExplosionEffect(effect.Size2, target->GetX(), target->GetY(),0,0,0);
	
	for(var o in FindObjects(Find_Distance(effect.Size2, target->GetX(), target->GetY()), Find_Func("CanBeHit")))
	{
		if(o->GetOwner() == target->GetOwner())
			continue;
			
		var angle = Angle(GetX(), GetY(), o->GetX(), o->GetY());
		
		o->Fling(Sin(angle, 8), -Cos(angle, 8));
		AddFireHitEffect(o);
		WeaponDamage(o, effect.SpellDamage2);
	}
	
	effect.marker->RemoveObject();
	//effect.clonk->MakeHitable(true);
	effect.clonk->Unstuck();
	effect.clonk->SetObjectLayer(nil);
}

