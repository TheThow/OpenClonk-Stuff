/**
	Terraria

	@author 
*/

func Initialize()
{
	CreateEnvironment();
}

func InitializePlayer(iPlr)
{
	var clonk = GetCrew(iPlr);
	var obj = clonk->CreateObject(Lorry, 0, 0, -1);
	obj->CreateContents(Tile_SolidBrick, 48*10);
	obj->CreateContents(Tile_WallBrick, 48*10);
	obj->CreateContents(Tile_SolidWood, 48*10);
	obj->CreateContents(Tile_WallWood, 48*10);
	obj->CreateContents(Tile_SolidMetal, 48*10);
	obj->CreateContents(Tile_WallMetal, 48*10);
	obj->CreateContents(Pickaxe, 5);
	obj->CreateContents(Shovel, 5);
	obj->CreateContents(Dynamite, 50);
	obj->CreateContents(Firestone, 50);
	obj->CreateContents(Hammer, 5);
	GivePlayerAllKnowledge(iPlr);
}

func CreateEnvironment()
{

}