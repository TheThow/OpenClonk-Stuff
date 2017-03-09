/* Automatically created objects file */

func InitializeObjects()
{
	CreateObjectAbove(JumpPad, 266, 748).Strength = 80;
	CreateObject(JumpPad, 480, 956).Strength = 70;
	var JumpPad001 = CreateObject(JumpPad, 709, 968);
	JumpPad001->SetR(45);
	JumpPad001.Strength = 80;
	var JumpPad002 = CreateObject(JumpPad, 804, 544);
	JumpPad002->SetR(-45);
	JumpPad002.Strength = 100;
	var JumpPad003 = CreateObject(JumpPad, 818, 646);
	JumpPad003->SetR(-82);
	JumpPad003.Strength = 80;
	var JumpPad004 = CreateObject(JumpPad, 708, 1094);
	JumpPad004->SetR(-45);
	JumpPad004.Strength = 80;
	var JumpPad005 = CreateObject(JumpPad, 760, 828);
	JumpPad005->SetR(94);
	JumpPad005.Strength = 80;	

	CreateObjectAbove(JumpPad, 1664 - 266, 748).Strength = 80;
	CreateObject(JumpPad, 1664 - 480, 956).Strength = 70;
	var JumpPad001 = CreateObject(JumpPad, 1664 - 709, 968);
	JumpPad001->SetR(-45);
	JumpPad001.Strength = 80;
	var JumpPad002 = CreateObject(JumpPad, 1664 - 804, 544);
	JumpPad002->SetR(45);
	JumpPad002.Strength = 100;		
	var JumpPad003 = CreateObject(JumpPad, 1664 - 818, 646);
	JumpPad003->SetR(82);
	JumpPad003.Strength = 80;
	var JumpPad004 = CreateObject(JumpPad, 1664 - 708, 1094);
	JumpPad004->SetR(45);
	JumpPad004.Strength = 80;
	var JumpPad005 = CreateObject(JumpPad, 1664 - 760, 828);
	JumpPad005->SetR(-94);
	JumpPad005.Strength = 80;		
	
	CreateObjectAbove(JumpPad, 832, 988);

	CreateObjectAbove(DefenseTower, 225, 683);
	CreateObjectAbove(DefenseTower, 586, 956);
	CreateObjectAbove(DefenseTower, 653, 701);
	CreateObjectAbove(DefenseTower, 581, 612);
	
	CreateObjectAbove(DefenseTower, 1664 - 225, 683);
	CreateObjectAbove(DefenseTower, 1664 - 586, 956);
	CreateObjectAbove(DefenseTower, 1664 - 653, 701);
	CreateObjectAbove(DefenseTower, 1664 - 581, 612);
	
	CreateObjectAbove(WoodenBridge, 388, 755);
	CreateObjectAbove(WoodenBridge, 652, 620);
	CreateObjectAbove(WoodenBridge, 796, 756);
	CreateObjectAbove(WoodenBridge, 764, 996);

	CreateObjectAbove(WoodenBridge, 1664 - 388, 755);
	CreateObjectAbove(WoodenBridge, 1664 - 652, 620);
	CreateObjectAbove(WoodenBridge, 1664 - 796, 756);
	CreateObjectAbove(WoodenBridge, 1664 - 764, 996);
	
	for (var bridge in FindObjects(Find_ID(WoodenBridge)))
	{
		bridge->SetClrModulation(0xff00ffff);
		bridge->SetHalfVehicleSolidMask(true);
		bridge->SetCategory(C4D_Vehicle);
		bridge->MakeInvincible(true);
	}		
	return true;
}
