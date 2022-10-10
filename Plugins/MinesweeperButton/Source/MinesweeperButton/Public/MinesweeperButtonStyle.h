// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

class FMinesweeperButtonStyle
{
public:

	static void Load();
	static void Unload();


	static TSharedPtr< FSlateStyleSet > Get();
	static FName GetStyleSetName();
	static void CreateStyle();

private:

	static TSharedPtr< FSlateStyleSet > Style;
};