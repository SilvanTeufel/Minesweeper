// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SOverlay.h"
//#include "Dialogs/CustomDialog.h"
#include "MinesweeperButtonModel.h"
#include "Data.h"

class SBox;
class SColorBlock;
class SImage;
class STextBlock;


class SMinesweeperButtonCell : public SOverlay
{

public:

	SLATE_BEGIN_ARGS(SMinesweeperButtonCell)
		: _Model(nullptr)
		, _CellCoordinates({ 0, 0 })
	{ }
	SLATE_ARGUMENT(TSharedPtr< FMinesweeperButtonModel >, Model)
		SLATE_ARGUMENT(Coordinates, CellCoordinates)
		SLATE_END_ARGS()

    SMinesweeperButtonCell();
	void Construct(const FArguments& InArgs);
	void UpdateCell() const;	
	const struct Coordinates& GetCellCoordinates() const { return CellCoordinates; };

protected:

	TSharedPtr < FMinesweeperButtonModel > MinesweeperButtonModelPtr;
	Coordinates CellCoordinates;
	TSharedPtr < SColorBlock > BackgroundColorBlock;
	TSharedPtr < SColorBlock > ExBackgroundColorBlock;
	TSharedPtr < SBox > MineBox;
	TSharedPtr < STextBlock > SurroundingMinesTextBlock;
	TSharedPtr < SImage > ButtonImage;
};