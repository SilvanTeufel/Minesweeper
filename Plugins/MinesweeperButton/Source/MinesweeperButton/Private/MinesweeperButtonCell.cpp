
// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "MinesweeperButtonCell.h"
#include "MinesweeperButtonModel.h"
#include "MinesweeperButtonView.h"
#include "MinesweeperButtonStyle.h"
#include "Styling/SlateStyle.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"


#define LOCTEXT_NAMESPACE "FMinesweeperButtonModule"

SMinesweeperButtonCell::SMinesweeperButtonCell()
{
	MinesweeperButtonModelPtr = nullptr;
	CellCoordinates = { 0, 0 };
}

void SMinesweeperButtonCell::Construct(const FArguments& InArgs)
{
	MinesweeperButtonModelPtr = InArgs._Model;
	CellCoordinates = InArgs._CellCoordinates;

	TSharedPtr< FSlateStyleSet > StyleSet = FMinesweeperButtonStyle::Get();

	if (!StyleSet.IsValid()) { return; }

	BackgroundColorBlock = SNew(SColorBlock)
		.Color(StyleSet->GetColor(MB_BackgroundStyle));

	ExBackgroundColorBlock = SNew(SColorBlock)
		.Color(StyleSet->GetColor(MB_ExpBackgroundStyle))
		.Visibility(EVisibility::Hidden);

	MineBox = SNew(SBox)
		.Padding(8.f)
		.MaxDesiredWidth(FMinesweeperButtonView::CellSize - 16.f)
		.MaxDesiredHeight(FMinesweeperButtonView::CellSize - 16.f)
		.Visibility(EVisibility::Hidden)
		[
			SNew(SImage)
			.Image(StyleSet->GetBrush(MB_MineStyle))
		];

	SurroundingMinesTextBlock = SNew(STextBlock)
		.Text( LOCTEXT("0", "0") )
		.Font(StyleSet->GetFontStyle(MB_NextMineStyle))
		.ColorAndOpacity(StyleSet->GetColor(MB_NextMineTextStyle))
		.Visibility(EVisibility::Hidden);

	ButtonImage = SNew(SImage)
		.Image(StyleSet->GetBrush(MB_ButtonStyle));

	AddSlot(CellLevel::Background)[BackgroundColorBlock.ToSharedRef()];
	AddSlot(CellLevel::Exploded)[ExBackgroundColorBlock.ToSharedRef()];
	AddSlot(CellLevel::Mine)[MineBox.ToSharedRef()];
	AddSlot(CellLevel::CloseMines)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)[SurroundingMinesTextBlock.ToSharedRef()];
	AddSlot(CellLevel::UnRevealed)[ButtonImage.ToSharedRef()];

}

void setVisibilityBlock(TSharedPtr <SColorBlock> Wid, EVisibility Vis) {

	if (Wid && Wid->GetVisibility() != Vis) {
		Wid->SetVisibility(Vis);
	}
}

void setVisibilityBox(TSharedPtr <SBox> Wid, EVisibility Vis) {

	if (Wid && Wid->GetVisibility() != Vis) {
		Wid->SetVisibility(Vis);
	}
}

void setVisibilityTextBlock(TSharedPtr <STextBlock> Wid, EVisibility Vis) {

	if (Wid && Wid->GetVisibility() != Vis) {
		Wid->SetVisibility(Vis);
	}
}

void setVisibilityImage(TSharedPtr <SImage> Wid, EVisibility Vis) {

	if (Wid && Wid->GetVisibility() != Vis) {
		Wid->SetVisibility(Vis);
	}
}

void SMinesweeperButtonCell::UpdateCell() const
{
	if (!MinesweeperButtonModelPtr.IsValid()) { return; }

	TSharedPtr< class FSlateStyleSet > StyleSet = FMinesweeperButtonStyle::Get();

	if (!StyleSet.IsValid()) { return; }

	const CellData& CellData = MinesweeperButtonModelPtr->CellCoordinatesToInfo(CellCoordinates);


	switch (CellData.State)
	{
	case Hide:
		setVisibilityBlock(BackgroundColorBlock, EVisibility::Visible);
		setVisibilityBlock(ExBackgroundColorBlock, EVisibility::Hidden);
		setVisibilityBox(MineBox, EVisibility::Hidden);
		setVisibilityTextBlock(SurroundingMinesTextBlock, EVisibility::Hidden);
		setVisibilityImage(ButtonImage, EVisibility::Visible);

		break;

	case Reveal:

		setVisibilityBlock(BackgroundColorBlock, EVisibility::Visible);
		setVisibilityBlock(ExBackgroundColorBlock, EVisibility::Hidden);
		setVisibilityBox(MineBox, (CellData.bIsMine ? EVisibility::Visible : EVisibility::Hidden));
		setVisibilityTextBlock(SurroundingMinesTextBlock, EVisibility::Hidden);
		setVisibilityImage(ButtonImage, EVisibility::Hidden);

		break;

	case Explode:

		setVisibilityBlock(BackgroundColorBlock, EVisibility::Visible);
		setVisibilityBlock(ExBackgroundColorBlock, EVisibility::Hidden);
		setVisibilityBox(MineBox, (CellData.bIsMine ? EVisibility::Visible : EVisibility::Hidden));
		setVisibilityTextBlock(SurroundingMinesTextBlock, EVisibility::Hidden);
		setVisibilityImage(ButtonImage, EVisibility::Hidden);

		break;
	}

	if (CellData.State == Reveal && !CellData.bIsMine)
	{
		uint16 NearbyMineCount = MinesweeperButtonModelPtr->GetSurroundingMineCount(CellCoordinates);

		if (NearbyMineCount > 8)
		{
			NearbyMineCount = 8;
		}else if (NearbyMineCount > 0)
		{
			SurroundingMinesTextBlock->SetText(FText::FromString(FString::FromInt(NearbyMineCount)));
			SurroundingMinesTextBlock->SetColorAndOpacity(StyleSet->GetColor(
				FName(MB_NextMineTextStyle)
			));
			SurroundingMinesTextBlock->SetVisibility(EVisibility::Visible);
		}
	}
}

#undef LOCTEXT_NAMESPACE
