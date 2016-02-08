/**
	BallAttackOrder
	

	@author 
*/

local ManaCost = 15;

func Launch(object clonk, int x, int y, proplist params)
{
	params.ball->AttackOrder(x, y);
	RemoveObject();
}