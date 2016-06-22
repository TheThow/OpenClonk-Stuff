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
	var obj = CreateObject(Lorry, LandscapeWidth()/2, 50, -1);
	obj->CreateContents(Tile_SolidBrick, 48*5);
	obj->CreateContents(Tile_WallBrick, 48*5);
	obj->CreateContents(Tile_SolidWood, 48*5);
	obj->CreateContents(Tile_WallWood, 48*5);
	obj->CreateContents(Pickaxe, 5);
	obj->CreateContents(Shovel, 5);
}