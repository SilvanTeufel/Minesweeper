// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "MinesweeperButtonTab.h"
#include "MinesweeperButtonModule.h"
#include "MinesweeperButtonController.h"
#include "MinesweeperButtonView.h"
#include "MinesweeperButtonStyle.h"
#include "Styling/SlateStyle.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SCanvas.h"
#include "Data.h"
#define LOCTEXT_NAMESPACE "FMinesweeperButtonModule"

const FName SMinesweeperButtonTab::TabName("Minesweeper");


SMinesweeperButtonTab::SMinesweeperButtonTab() {
	View = nullptr;
	WidthSlider = nullptr;
	HeightSlider = nullptr;
	CountSlider = nullptr;
	ResetButton = nullptr;
	GameWindow = nullptr;
	FieldContainer = nullptr;
	Field = nullptr;

}


void SMinesweeperButtonTab::Construct(const FArguments& InArgs)
{
	View = InArgs._View;
	if (!View.IsValid()) { return; }
	TSharedPtr< class FSlateStyleSet > StyleSet = FMinesweeperButtonStyle::Get();
	if (!StyleSet.IsValid()) { return; }
	SDockTab::Construct(
		SDockTab::FArguments()
		.TabRole(NomadTab)
		.OnTabClosed(View.Get(), &FMinesweeperButtonView::OnTabClosed)
		[
			SAssignNew(GameWindow, SBox)
			[
				SNew(SVerticalBox)


		/// MINESWEEPER GRID
		+SVerticalBox::Slot()
		.Padding(10.f, 10.f, 10.f, 10.f)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth()
			.Padding(MB_Margrin_Grid)
			[
				SAssignNew(FieldContainer, SBox)
				[
					SAssignNew(Field, SCanvas)
				]
			]
		]
		+ SVerticalBox::Slot()
			.MaxHeight(25.f)
			.Padding(10.f, 10.f, 10.f, 10.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
					.Padding(FMargin(10.f, 0.f, 0.f, 0.f))
					.AutoWidth()
					[
						SAssignNew(ResetButton, SButton)
						.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Text(LOCTEXT("Button", "Reset"))
					]
				+ SHorizontalBox::Slot()
					.Padding(FMargin(10.f, 0.f, 0.f, 0.f))
					.AutoWidth()
					[
						SNew(STextBlock)
						.Font(StyleSet->GetFontStyle(MB_MenuButtonStyle))
					.Text(this, &SMinesweeperButtonTab::CreateGameText)
					]
			]
		+ SVerticalBox::Slot()
				.MaxHeight(25.f)
				.Padding(10.f, 10.f, 10.f, 10.f)
				[
						SNew(SHorizontalBox)
				
				+ SHorizontalBox::Slot()
					.MaxWidth(100.f)
					[
					SAssignNew(CountSlider, SSpinBox< uint16 >)
					.MinSliderValue(1)
					.MaxSliderValue(this, &SMinesweeperButtonTab::GetMaxMineCount)
					.Value(View->GetMineCount())
					.OnValueChanged(View.Get(), &FMinesweeperButtonView::OnCountSliderValueChanged)
					]
				+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(FMargin(20.f, 4.f, 20.f, 0.f))
					[
						SNew(STextBlock)
						.Font(StyleSet->GetFontStyle(MB_MenuButtonStyle))
					.Text(LOCTEXT("MineCount", "MineCount"))
					]
				]
		+ SVerticalBox::Slot()
			.MaxHeight(25.f)
			.Padding(10.f, 10.f, 10.f, 10.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
					.MaxWidth(100.f)
					[
					SAssignNew(WidthSlider, SSpinBox< uint8 >)
					.MinSliderValue(1)
					.MaxSliderValue(this, &SMinesweeperButtonTab::GetMaxFieldWidth)
					.Value(View->GetFieldWidth())
					.MinDesiredWidth(100.f)
					.OnValueChanged(View.Get(), &FMinesweeperButtonView::OnWidthSliderValueChanged)
					]
				+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(FMargin(20.f, 4.f, 20.f, 0.f))
					[
						SNew(STextBlock)
						.Font(StyleSet->GetFontStyle(MB_MenuButtonStyle))
					.Text(LOCTEXT("Width", "Width"))
					]
		]
		+ SVerticalBox::Slot()
			.MaxHeight(25.f)
			.Padding(10.f, 10.f, 10.f, 10.f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
					.MaxWidth(100.f)
					[
					SAssignNew(HeightSlider, SSpinBox< uint8 >)
					.MinSliderValue(1)
					.MaxSliderValue(this, &SMinesweeperButtonTab::GetMaxFieldHeight)
					.Value(View->GetFieldHeight())
					.MinDesiredWidth(100.f)
					.OnValueChanged(View.Get(), &FMinesweeperButtonView::OnHeightSliderValueChanged)
					]
				+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(FMargin(20.f, 4.f, 20.f, 0.f))
					[
						SNew(STextBlock)
						.Font(StyleSet->GetFontStyle(MB_MenuButtonStyle))
					.Text(LOCTEXT("Height", "Height"))
					]
			]

		]
		]
	);
}


TSharedPtr< SSpinBox< uint8 > > SMinesweeperButtonTab::GetWidthSlider() const
{
	return WidthSlider;
};
TSharedPtr< SSpinBox< uint8 > > SMinesweeperButtonTab::GetHeightSlider() const
{
	return HeightSlider;
};
TSharedPtr< SSpinBox< uint16 > > SMinesweeperButtonTab::GetCountSlider() const
{
	return CountSlider;
};
TSharedPtr< SButton > SMinesweeperButtonTab::GetResetButton() const
{
	return ResetButton;
};
TSharedPtr< SBox > SMinesweeperButtonTab::GetFieldContainer() const 
{
	return FieldContainer;
};
TSharedPtr< SBox > SMinesweeperButtonTab::GetSettingsContainer() const 
{
	return SettingsContainer;
};
TSharedPtr< SCanvas > SMinesweeperButtonTab::GetField() const
{
	return Field;
};

TOptional< uint8 > SMinesweeperButtonTab::GetMaxFieldWidth() const
{
	if (!GameWindow.IsValid())
		return 255;

	float Width = GameWindow->GetCachedGeometry().GetLocalSize().X - 20.f;

	if (Width < FMinesweeperButtonView::CellSize)
		return 255;

	Width /= FMinesweeperButtonView::CellSize;

	if (Width < 1.f)
		Width = 1.f;

	else if (Width > 255.f)
		Width = 255.f;

	return static_cast<uint8>(Width);
}

TOptional< uint8 > SMinesweeperButtonTab::GetMaxFieldHeight() const
{
	if (!GameWindow.IsValid())
		return 255;

	float Height = GameWindow->GetCachedGeometry().GetLocalSize().Y - 65.f;

	if (Height < FMinesweeperButtonView::CellSize)
		return 255;

	Height /= FMinesweeperButtonView::CellSize;

	if (Height < 1.f)
		Height = 1.f;

	else if (Height > 255.f)
		Height = 255.f;

	return static_cast<uint8>(Height);
}

TOptional<uint16> SMinesweeperButtonTab::GetMaxMineCount() const
{


	uint8 height = View->GetFieldHeight();
	uint8 width = View->GetFieldWidth();
	return static_cast<uint16>(height*width/2);
}

FText SMinesweeperButtonTab::GetMineCount() const
{
	uint16 MineCount = GetCountSlider()->GetValue();

	return FText::FromString(FString::FromInt(MineCount));
}

FText SMinesweeperButtonTab::CreateGameText() const
{

	FMinesweeperButtonModule* MinesweeperButtonModulePtr = FMinesweeperButtonModule::GetPtr();

	if (!MinesweeperButtonModulePtr || !MinesweeperButtonModulePtr->GetController().IsValid())
		return LOCTEXT("Somehting went wrong", "Error");


	switch (MinesweeperButtonModulePtr->GetController()->GetGameStatus())
	{
	case Stop:

		return LOCTEXT("Stop", "Stop");
		break;
	case Gaming:

		return LOCTEXT("Gaming", "Gaming");
		break;
	case Lose:

		return LOCTEXT("Lose", "Lose");
		break;
	case Win:

		return LOCTEXT("Win", "Win");
		break;
	case Error:
		return LOCTEXT("Error", "Error");
		break;
	}


	return LOCTEXT("Error", "Error");
}

void SMinesweeperButtonTab::CreatePadding() const
{
	FMinesweeperButtonModule* MinesweeperButtonModulePtr = FMinesweeperButtonModule::GetPtr();

	if (GameWindow.IsValid() && MinesweeperButtonModulePtr && MinesweeperButtonModulePtr->GetModel().IsValid())
	{
		float TotalWidth = GameWindow->GetCachedGeometry().GetLocalSize().X - 20.f;
		float WidthPadding = TotalWidth - (MinesweeperButtonModulePtr->GetModel()->GetFieldWidth() * FMinesweeperButtonView::CellSize);

		if (WidthPadding < 0.f)
			WidthPadding = 0.f;

		float TotalHeight = GameWindow->GetCachedGeometry().GetLocalSize().Y - 65.f;
		float HeightPadding = TotalHeight - (MinesweeperButtonModulePtr->GetModel()->GetFieldHeight() * FMinesweeperButtonView::CellSize);

		if (HeightPadding < 0.f)
			HeightPadding = 0.f;

		FieldContainer->SetPadding(FMargin(
			WidthPadding / 2.f,
			HeightPadding / 2.f,
			WidthPadding / 2.f,
			HeightPadding / 2.f
		));
	}
	
}

#undef LOCTEXT_NAMESPACE
