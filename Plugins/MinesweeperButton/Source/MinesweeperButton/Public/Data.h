// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"



UENUM()
enum CellLevel
{
	Background,
	Exploded,
	Mine,
	CloseMines,
	UnRevealed,
};

UENUM()
enum CellState
{
	Hide,
	Reveal,
	Explode
};

UENUM()
enum MB_GameState
{
		Stop,
		Gaming,
		Lose,
		Win,
		Error
};

#define MB_StyleName FName("MinesweeperButtonStyle");
#define MB_ToolsButtonStyle "MinesweeperButton.CommandInfo"
#define MB_MenuButtonStyle "MinesweeperButtonStyle.MenuButton"
#define MB_ButtonStyle "MinesweeperButtonStyle.Button"
#define MB_MineStyle "MinesweeperButtonStyle.Mine"
#define MB_BackgroundStyle "MinesweeperButtonStyle.Background"
#define MB_ExpBackgroundStyle "MinesweeperButtonStyle.ExBackground"
#define MB_MenuStyle "MinesweeperButtonStyle.Menu"
#define MB_NextMineStyle "MinesweeperButtonStyle.NextMine"
#define MB_NextMineTextStyle "MinesweeperButtonStyle.NextMineTextColour"
#define MB_Field_width 15
#define MB_Field_height 15
#define MB_Field_Minecount 10

#define Path_Button_Icon TEXT("/Engine/MobileResources/HUD/MobileHUDDirectionStick.MobileHUDDirectionStick")
#define Path_Mine_Icon TEXT("/Engine/MobileResources/HUD/MobileHUDButtonFire.MobileHUDButtonFire")

#define MB_Margrin_Grid FMargin(40.f, 10.f, 10.f, 10.f)
