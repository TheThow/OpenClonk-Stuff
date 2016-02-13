/**
	BallAttackOrder

	@author 
*/

local ManaCost = 35;

func Launch(object clonk, int x, int y, proplist params)
{
	params.ball->Discharge(x, y);
	RemoveObject();
}