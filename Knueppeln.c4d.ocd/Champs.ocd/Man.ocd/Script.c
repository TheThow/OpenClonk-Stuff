/**
	Man
	

	@author 
*/

local Special1Cooldown = 10;
local Special2Cooldown = 10;
local Special3Cooldown = 10;

local Special1Spell = Projectile;
local Special2Spell = Projectile;
local Special3Spell = Projectile;

local SpecialMeleeFx = new Effect {

	Construction = func()
	{
		this.x = Target->GetX();
		this.y = Target->GetY();
	},

	Timer = func()
	{
		if(this.r)
			return -1;
	
		this.timer++;
		//Target->CreateParticle("Flash", 0, 0, 0, 0, 15, this.p, 1);
		Target->DrawParticleLine("Flash", 0, 0, this.x - Target->GetX(), this.y - Target->GetY(), 1, 0, 0, 15, this.p);
		
		for (var obj in Target->FindObjects(Find_Distance(15), Find_Exclude(Target), Find_Func("CanBeHit", Target)))
		{
			if(GetEffect(Format("SpecialMeleeHitCD%d", Target->GetOwner()), obj))
				continue;
			
			var fx = GetEffect("SpecialMeleeFx", obj);
			if(fx)
			{
				fx.r = true;
				RemoveEffect("SpecialMeeleFx", obj);
				var a = Angle(Target->GetX(), Target->GetY(), obj->GetX(), obj->GetY());
				Target->Sound("Hits::Materials::Metal::LightMetalHit?");
				
				Target->Fling(Sin(a - 180, 5), -Cos(a - 180, 5));
				obj->Fling(Sin(a, 5), -Cos(a, 5));
				AddEffect(Format("SpecialMeleeHitCD%d", obj->GetOwner()), Target, 1, 1);
				this.r = true;
				return -1;
			}
			
			Target.ChampType->SpecialMeleeStrike(Target, obj);
			AddEffect(Format("SpecialMeleeHitCD%d", Target->GetOwner()), obj, 1, 11);
		}
		
		this.x = Target->GetX();
		this.y = Target->GetY();
		
		if(this.timer > 10)
			return -1;
	},
	
	Damage = func(dmg)
	{
		if(dmg < 0)
			this.r = true;
	},
	
	Stop = func()
	{
		if(Target->GetAction("Travel"))
		{
			Target->SetAction("Jump");
			var a = Angle(0, 0, Target->GetXDir(), Target->GetYDir());
			Target->SetVelocity(a, 30);
		}
	}

};


/*
Parameters: 
clonk 			Calling clonk, 
x		 		mouse x, 
y 				mouse y, 
released		key/mouse event released, 
mouseclick 		mouseclick when the special key is pressed, 
abletocast 		clonk is able to cast at the moment, 
cooldown 		spell is on cooldown
*/
func Special1(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	return 0;
}

func Special2(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	return 0;
}

func Special3(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	return 0;
}

/*
dir 	"Up", "Left", "Right", "Down"
*/
func JumpEffect(object clonk, string dir)
{

}

/*
range	range of the block effect
*/
func BlockEffect(object clonk, int range)
{

}

func LeftClick(object clonk, int x, int y, bool released, bool abletocast)
{

}

func InitChamp(object clonk)
{

}

func CanCast(object clonk)
{
	return true;
}

func CanCastSpecial1(object clonk)
{
	return true;
}

func CanCastSpecial2(object clonk)
{
	return true;
}

func CanCastSpecial3(object clonk)
{
	return true;
}

func CanCastSpecialMelee(object clonk)
{
	return true;
}

func CleanUp(object clonk)
{

}

func CastSpellWithSpellRange(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown, proplist props, id spell)
{
	if(!released && !mouseclick)
	{
		clonk->ShowSpellRange(clonk, spell, props);
	}
	
	if(released && !mouseclick)
	{
		clonk->CancelShowSpellRange();
	}

	if(!released && mouseclick && abletocast && !cooldown)
	{
		if(!CastSpellWithSpellRangeCondition(clonk, x, y, released, mouseclick, abletocast, cooldown, props, spell))
		{
			Sound("UI::Error", false, 50, clonk->GetOwner());
			return 0;
		}
	
		if (Sqrt(x**2 + y**2) > spell.SpellRange)
		{
			var a = Angle(0,0, x, y, 10);
			var newx = Sin(a, spell.SpellRange, 10);
			var newy = -Cos(a, spell.SpellRange, 10);
			
			if(clonk->LaunchSpell(spell, newx, newy, newx, newy))
				return 1;
			
			return 0;
		}
		
		if(clonk->LaunchSpell(spell, x, y, x, y))
			return 1;
	}
	
	return 0;
}

func CastSpellWithSpellRangeCondition(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown, proplist props, id spell)
{
	return true;
}

func SpecialMeleeAttack(object clonk, int x, int y, bool released, bool mouseclick, bool abletocast, bool cooldown)
{
	if (!abletocast || cooldown || released || mouseclick)
		return false;
	
	var a = Angle(0, 0, x, y);

	clonk->SetAction("Travel");
	clonk->SetVelocity(a, 55);
	
	var rgba = SplitRGBaValue(GetPlayerColor(clonk->GetOwner()));
	
	var props = {
		R = rgba[0],
		G = rgba[1],
		B = rgba[2],
		Size = PV_Linear(4, 0),
		Alpha = PV_Linear(255, 0),
		BlitMode = GFX_BLIT_Additive,
		Attach = ATTACH_Back
	};
	
	var fx = clonk->CreateEffect(SpecialMeleeFx, 1, 1);
	fx.p = props;
	
	var sword = clonk->FindContents(Sword);
	
	var length = Sword_Standard_StrikingLength;
	var arm = "R";
	var animation = Format("SwordSlash%d.%s", 1, arm);
	var animation_sword = Format("Strike%d", 1);
	
	sword->PlayWeaponAnimation(clonk, animation, 10, Anim_Linear(0, 0, clonk->GetAnimationLength(animation), length, ANIM_Remove), Anim_Const(1000));
	sword->PlayAnimation(animation_sword, 10, Anim_Linear(0, 0, clonk->GetAnimationLength(animation_sword), length, ANIM_Remove), Anim_Const(1000));
	
	clonk->Sound("Objects::Weapons::WeaponSwing?");
	
	return true;
}
func SpecialMeleeStrike(clonk, target)
{
	clonk->WeaponDamage(target, 25);
	target->Fling(0, -2);
	clonk->Sound("Objects::Weapons::WeaponHit?", false);
}