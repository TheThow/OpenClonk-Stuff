static team_exclusiveChampions;
static lastChosenChampion;
static banned_Champions;

global func GetChampions()
{
	var Champ_Def = [ArchMan, BallsMan, BatMan, BombMan, EarthMan, ElectroMan, FireMan, FlashMan, GreatLeaderMan, HookMan, IceMan, LaserMan, NinjaMan, TimeMan];
	return Champ_Def;
}

global func GetBannedChampions()
{
	if (banned_Champions == nil)
		return [];
		
	return banned_Champions;
}

global func BanChampion(id champ)
{
	if (banned_Champions == nil)
		banned_Champions = [];
	
	PushBack(banned_Champions, champ);
}

global func GetChampionsWithoutBanned()
{
	if (banned_Champions == nil)
		return GetChampions();
	
	var champs = [];
	for (var champ in GetChampions())
	{
		var flag = false;
	
		for (var champ2 in banned_Champions)
		{
			if (champ == champ2)
			{
				flag = true;
				break;
			}
		}
		
		if (flag)
			continue;
		
		PushBack(champs, champ);
	}
	
	return champs;
}

// Sets an objects's speed and its direction, doesn't it?
// Can set either speed or angle of velocity, or both
global func AddVelocity(int angle, int speed, int precAng, int precSpd)
{
	if(!precSpd) precSpd = 10;
	if(!precAng) precAng = 1;
	if(!speed)
		speed = Distance(0,0, GetXDir(precSpd), GetYDir(precSpd));
	if(!angle)
		angle = Angle(0,0, GetXDir(precSpd), GetYDir(precSpd), precAng);
		
	var x_dir = this->GetXDir(precSpd) + Sin(angle, speed, precAng);
	var y_dir = this->GetYDir(precSpd) - Cos(angle, speed, precAng);

	SetXDir(x_dir, precSpd);
	SetYDir(y_dir, precSpd);
	return;
}

global func UpdateAllSelectionMenus()
{
	for(var o in FindObjects(Find_ID(Clonk), Find_OCF(OCF_Alive)))
	{
		o->UpdateSelectionMenu();
	}
}

global func SetLastChosenChampion(int plr, id champ)
{
	if(lastChosenChampion == nil)
		lastChosenChampion = CreateArray(GetPlayerCount()*2);
		
	lastChosenChampion[plr] = champ;
}

global func GetLastChosenChampion(int plr)
{
	if(lastChosenChampion == nil)
		return nil;
		
	return lastChosenChampion[plr];
}

global func InitTeamExclusiveChamps(int teamcount)
{
	team_exclusiveChampions = CreateArray(teamcount);
	
	for(var i = 0; i < GetLength(team_exclusiveChampions); i++)
	{
		team_exclusiveChampions[i] = [];
	}
}

global func RemoveTeamChamp(int teamid, id ChampType)
{
	var banned = team_exclusiveChampions[teamid -1];
	var newb = CreateArray(GetLength(banned) + 1);
	
	for(var i = 0; i < GetLength(banned); i++)
	{
		newb[i] = banned[i];
	}
	
	newb[GetLength(newb) - 1] = ChampType;
	team_exclusiveChampions[teamid -1] = newb;
}

global func ResetTeamExclusiveChamps()
{
	for(var i = 0; i < GetLength(team_exclusiveChampions); i++)
	{
		team_exclusiveChampions[i] = [];
	}
}

global func GetBannedTeamChampions(int teamid)
{
	if (team_exclusiveChampions == nil)
		return [];
	
	return team_exclusiveChampions[teamid -1];
}

global func GetCurrentBannedTeamChampions(int teamid)
{
	var list = [];
	
	for (var o in FindObjects(Find_ID(Clonk), Find_OCF(OCF_Alive), Find_AnyLayer()))
	{
		if (GetPlayerTeam(o->GetOwner()) == teamid)
		{
			if (o.ChampType != Man)
				PushBack(list, o.ChampType);
		}
	}
	
	return list;
}


global func DrawLightningSmall(fromX, fromY, toX, toY, props)
{
	props = props ??
	{
		Size = 10, // will be overwritten
		R = 255, G = 255, B = 255,
		Alpha = PV_KeyFrames(0, 0, 200, 100, 0, 200, 255, 1000, 0),
		Phase = PV_Random(0, 5),
		BlitMode = GFX_BLIT_Additive
	};
	var a = Angle(fromX, fromY, toX, toY);
	var d = Distance(fromX, fromY, toX, toY);
	props.Rotation = a;
	props.Size = d;
	CreateParticle("Lightning", (fromX + toX) / 2, (fromY + toY) / 2, 0, 0, PV_Random(15, 25), props, 1);
}

global func DrawLightning(fromX, fromY, toX, toY, props)
{
	props = props ??
	{
		Size = 10, // will be overwritten
		R = 255, G = 255, B = 255,
		Alpha = PV_KeyFrames(0, 0, 200, 100, 0, 200, 255, 1000, 0),
		Phase = PV_Random(0, 5),
		BlitMode = GFX_BLIT_Additive
	};
	var lastX,lastY;
    var x1=fromX;
    var y1=fromY;
    var x2=toX;
    var y2=toY;
    
    lastX=x1;
    lastY=y1;
    
    var i, deltax, deltay, numpixels;
    var d, dinc1, dinc2;
    var x, xinc1, xinc2;
    var y, yinc1, yinc2;

    deltax = Abs(x2 - x1);
    deltay = Abs(y2 - y1);

    if(deltax >= deltay) {
        numpixels = deltax + 1;
        d = (2 * deltay) - deltax;
        dinc1 = deltay << 1;
        dinc2 = (deltay - deltax) << 1;
        xinc1 = 1;
        xinc2 = 1;
        yinc1 = 0;
        yinc2 = 1;
    } else {
        numpixels = deltay + 1;
        d = (2 * deltax) - deltay;
        dinc1 = deltax << 1;
        dinc2 = (deltax - deltay) << 1;
        xinc1 = 0;
        xinc2 = 1;
        yinc1 = 1;
        yinc2 = 1;
    }

    if(x1 > x2) {
        xinc1 = -xinc1;
        xinc2 = -xinc2;
    }
    if(y1 > y2) {
        yinc1 = -yinc1;
        yinc2 = -yinc2;
    }

    x=x1;
    y=y1;
	
	var last = 1;
    for(i = 1; i < numpixels; i++) {
        if((!Random(25)) || (last < i - 12) || ((i % (numpixels - 1)/2) == 0))
        {
        	last = i;
        	var aX=x+RandomX(-10,10);
        	var aY=y+RandomX(-10,10);
        	DrawLightningSmall(lastX,lastY,aX,aY, props);
        	lastX=aX;
        	lastY=aY;
        }

        if(d < 0) {
            d = d + dinc1;
            x = x + xinc1;
            y = y + yinc1;
        } else {
            d = d + dinc2;
            x = x + xinc2;
            y = y + yinc2;
        }
    }
}

global func Unstuck()
{
	if(!this->Stuck())
		return;

	var flag = false;
	
	var ix = GetX();
	var iy = GetY();

	for(var i = 0; i < 40; i += 5)
	{
		for(var r = 0; r < 360; r += 10)
		{
			var x = Sin(r, i);
			var y = -Cos(r, i);
		
			if(!GBackSolid(x, y))
			{
				SetPosition(ix + x, iy + y);
				
				if(!this->Stuck())
				{
					flag = true;
					break;
				}
			}

		}
		
		if(flag)
			break;
	}
	
}