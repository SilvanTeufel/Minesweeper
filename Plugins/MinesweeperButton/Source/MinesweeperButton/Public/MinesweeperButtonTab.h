
// Copyright 

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Docking/SDockTab.h"


class FMinesweeperButtonView;
class SBox;
class SButton;
class SCanvas;
class SSlider;
template<typename NumericType>
class SSpinBox;
struct FSlateColor;

class SMinesweeperButtonTab : public SDockTab
{

public:

	SLATE_BEGIN_ARGS(SMinesweeperButtonTab)
		: _View(nullptr)
	{ }
	SLATE_ARGUMENT(TSharedPtr< FMinesweeperButtonView >, View)
		SLATE_END_ARGS()

		static const FName TabName;

	SMinesweeperButtonTab();

	void Construct(const FArguments& InArgs);

	// UI FUNCTIONS //////////////////////////////////////////////////////////////////
	TSharedPtr< SSpinBox< uint8 > > GetWidthSlider() const;
	TSharedPtr< SSpinBox< uint8 > > GetHeightSlider() const;
	TSharedPtr< SSpinBox< uint16 > > GetCountSlider() const;
	TSharedPtr< SButton > GetResetButton() const;
	TSharedPtr< SBox > GetFieldContainer() const;
	TSharedPtr< SBox > GetSettingsContainer() const;
	TSharedPtr< SCanvas > GetField() const;
	////////////////////////////////////////////////////////////////////////////////////


	void CreatePadding() const;

protected:

	/// GENERATE VALUES FOR UI //////////////////////////////
	TOptional< uint8 > GetMaxFieldWidth() const;
	TOptional< uint8 > GetMaxFieldHeight() const;
	TOptional< uint16 > GetMaxMineCount() const;
	FText GetMineCount() const;
	FText CreateGameText() const;
	////////////////////////////////////////////////////////////////////////////////////

	/// UI ELEMENTS ////////////////////////////////////////////////////////
	TSharedPtr< FMinesweeperButtonView > View;
	TSharedPtr< SSpinBox< uint8 > > WidthSlider;
	TSharedPtr< SSpinBox< uint8 > > HeightSlider;
	TSharedPtr< SSpinBox< uint16 > > CountSlider;
	TSharedPtr< SButton > ResetButton;
	TSharedPtr< SBox > GameWindow;
	TSharedPtr< SBox > FieldContainer;
	TSharedPtr< SBox > SettingsContainer;
	TSharedPtr< SCanvas > Field;
	////////////////////////////////////////////////////////////////////////////////////
};