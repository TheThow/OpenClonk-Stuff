/**
	Terraria

	@author 
*/

func Initialize()
{
	CreateEnvironment();
}

func CreateEnvironment()
{
	var obj = CreateObject(Lorry, 100, 50, -1);
	obj->CreateContents(Tile_SolidBrick, 48*5);
	obj->CreateContents(Tile_WallBrick, 48*5);
	obj->CreateContents(Pickaxe, 5);
	obj->CreateContents(Shovel, 5);
}