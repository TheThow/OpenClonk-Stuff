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