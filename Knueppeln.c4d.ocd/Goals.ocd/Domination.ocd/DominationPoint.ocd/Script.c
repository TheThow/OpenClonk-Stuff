
/*-- Initialize --*/

local bar;
local owner;
local curr_capturer;

local frames_to_capture = 100;
local capture;

local prt_props;

local CheckFx = new Effect {

	Timer = func()
	{
		this.timer++;
		
		var r = 5;
		this.timer2 += r;
		Target->CreateParticle("Flash", Cos(this.timer2, 20), Sin(this.timer2, 20), 0, 0, 360/r, Target.prt_props, 1);
		

		var a = CreateArray(GetStartupTeamCount() + 1);
		
		for (var o in Target->FindObjects(Find_Distance(20), Find_ID(Clonk), Find_OCF(OCF_Alive)))
		{
			a[GetPlayerTeam(o->GetOwner())]++;
		}
		
		var team = -1;
		var c = 0;
		for (var i = 1; i < GetLength(a); i++)
		{
			if (a[i] == c)
			{
				team = -1;
			}
		
		
			if (a[i] > c)
			{
				team = i;
				c = a[i];
			}
		}
		
		if (team == -1)
		{
			return;
		}
		
		if (!Target.curr_capturer && !Target.owner)
		{
			Target.curr_capturer = team;

			var rgba = SplitRGBaValue(GetTeamColor(team));
			Target.bar->SetBarColor(RGBa(rgba[0], rgba[1], rgba[2], 255));
		}
		
		if (!Target.curr_capturer && Target.owner && Target.owner != team)
		{
			Target.curr_capturer = team;
			
			var rgba = SplitRGBaValue(GetTeamColor(Target.owner));
			Target.bar->SetBarColor(RGBa(rgba[0], rgba[1], rgba[2], 255));
		}
		
		if (Target.curr_capturer && Target.curr_capturer == team && !Target.owner)
		{
			Target.capture++;
			Target.bar->SetValue(Target.capture);
		}
		
		if (Target.owner && Target.owner != team && Target.capture > 0)
		{
			Target.capture--;
			Target.bar->SetValue(Target.capture);
			
			if (Target.capture <= 0)
				Scenario->~OnCapturePointLiberated(this.Target, team, Target.owner);
		}
		
		if (!Target.owner && Target.curr_capturer != team)
		{
			Target.capture--;
			Target.bar->SetValue(Target.capture);
		}
		
		if (Target.owner == team)
		{

			if (Target.capture < Target.frames_to_capture && this.timer%5 == 0)
			{
				Target.capture++;
				Target.bar->SetValue(Target.capture);
			}
		}
		
		if(Target.capture <= 0)
		{
			Target.capture = 0;
			Target.curr_capturer = nil;
			Target.owner = nil;
			Target->SetClrModulation(RGBa(255, 255, 255, 160) , 1);
			
			Target.prt_props.R = 255;
			Target.prt_props.G = 255;
			Target.prt_props.B = 255;
			
		}
		
		if (Target.capture >= Target.frames_to_capture && !Target.owner)
		{
			Target.owner = team;
			
			var rgba = SplitRGBaValue(GetTeamColor(Target.owner));
			Target.prt_props.R = rgba[0];
			Target.prt_props.G = rgba[1];
			Target.prt_props.B = rgba[2];
			
			Target.curr_capturer = nil;
			Target.capture = Target.frames_to_capture;
			Target->SetClrModulation(GetTeamColor(Target.owner) , 1);
			
			Scenario->~OnCapturePointCaptured(this.Target, team);
		}
		
	}

};


protected func Initialize()
{
	curr_capturer = nil;
	owner = nil;
	bar = nil;

	SetGraphics("", ParkourFlag, 1, GFXOV_MODE_Base);
	SetObjDrawTransform(350, 0, 2000, 0, 350, 2000, 1);
	SetClrModulation(RGBa(255, 255, 255, 160) , 1);
	
	bar = CreateProgressBar(GUI_SimpleProgressBar, frames_to_capture, 0, nil, -1, {x=0, y=-25});

	CreateEffect(CheckFx, 1, 1);
	
	prt_props = {
		Size = PV_Linear(5, 0),
		Alpha = PV_Linear(255, 0),
		BlitMode = GFX_BLIT_Additive,
	};
	
	return;
}

public func GetTeam()
{
	return owner;
}

