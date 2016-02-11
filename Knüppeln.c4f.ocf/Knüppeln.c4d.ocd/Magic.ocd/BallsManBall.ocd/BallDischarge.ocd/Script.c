/**
	BallAttackOrder

	@author 
*/

local ManaCost = 37;

func Launch(object clonk, int x, int y, proplist params)
{
	params.ball->Discharge(x, y);
	RemoveObject();
}