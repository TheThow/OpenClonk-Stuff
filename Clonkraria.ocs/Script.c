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
	var obj = clonk->CreateObject(Chest, 0, 0, -1);
	obj->CreateContents(Tile_SolidBrick)->SetStackCount(999);
	obj->CreateContents(Tile_WallBrick)->SetStackCount(999);
	obj->CreateContents(Tile_SolidWood)->SetStackCount(999);
	obj->CreateContents(Tile_WallWood)->SetStackCount(999);
	obj->CreateContents(Tile_SolidMetal)->SetStackCount(999);
	obj->CreateContents(Tile_WallMetal)->SetStackCount(999);
	obj->CreateContents(Tile_PillarWood)->SetStackCount(999);
	obj->CreateContents(Tile_Conveyor)->SetStackCount(999);
	obj->CreateContents(Pickaxe, 5);
	obj->CreateContents(Shovel, 5);
	obj->CreateContents(Dynamite, 50);
	obj->CreateContents(Firestone, 50);
	obj->CreateContents(Hammer, 5);
	obj->CreateObject(Lorry);
	GivePlayerAllKnowledge(iPlr);
}

func CreateEnvironment()
{

}