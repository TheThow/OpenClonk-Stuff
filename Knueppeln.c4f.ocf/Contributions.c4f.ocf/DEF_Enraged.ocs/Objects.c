/* Automatically created objects file */

func InitializeObjects()
{
	CreateObjectAbove(DefenseTower, 237, 342, 0);
	CreateObjectAbove(DefenseTower, 437, 238, 0);
	CreateObjectAbove(DefenseTower, 541, 406, 0);
	CreateObjectAbove(InventorsLab, 88, 417, 0);
	var JumpPad001 = CreateObject(JumpPad, 324, 431);
	JumpPad001.Strength = 75;
	JumpPad001->SetR(45);
	var JumpPad002 = CreateObject(JumpPad, 952, 216);
	JumpPad002->SetR(-45);
	JumpPad002.Strength = 75;
	var JumpPad003 = CreateObject(JumpPad, 413, 431);
	JumpPad003->SetR(-45);
	JumpPad003.Strength = 60;
	return true;
}
