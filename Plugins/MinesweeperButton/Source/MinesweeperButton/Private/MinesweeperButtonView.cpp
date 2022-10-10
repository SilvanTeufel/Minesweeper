

#pragma once

#include "MinesweeperButtonView.h"
#include "MinesweeperButtonModule.h"
#include "MinesweeperButtonController.h"
#include "MinesweeperButtonModel.h"
#include "MinesweeperButtonCell.h"
#include "MinesweeperButtonStyle.h"
#include "MinesweeperButtonTab.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SCanvas.h"


#define LOCTEXT_NAMESPACE "FMinesweeperButtonModule"
const float FMinesweeperButtonView::CellSize = 32.f;

FMinesweeperButtonView::FMinesweeperButtonView()
{

	FieldWidth = 1;
	FieldHeight = 1;
	FieldMineCount = 1.f;
	MouseDownKey = EKeys::Invalid;
	MouseDownCell.X = -1;
	MouseDownCell.Y = -1;
}



void FMinesweeperButtonView::Init()
{
	TSharedRef< FGlobalTabmanager > TabManager = FGlobalTabmanager::Get();
	TabManager->RegisterTabSpawner(SMinesweeperButtonTab::TabName, FOnSpawnTab::CreateRaw(this, &FMinesweeperButtonView::CreateTab));
	
}

void FMinesweeperButtonView::Destruct()
{
	TSharedRef< FGlobalTabmanager > TabManager = FGlobalTabmanager::Get();
	TabManager->UnregisterTabSpawner(SMinesweeperButtonTab::TabName);
	Tab.Reset();
}

TSharedRef<SDockTab> FMinesweeperButtonView::CreateTab(const FSpawnTabArgs& TabSpawnArgs)
{
	FMinesweeperButtonModule* MinesweeperButtonModule = FMinesweeperButtonModule::GetPtr();

	if (MinesweeperButtonModule) {
		TSharedRef< SMinesweeperButtonTab > NewTab = SNew(SMinesweeperButtonTab).View(MinesweeperButtonModule->GetView()); // Avoid making a new shared ptr.

		if (NewTab->GetResetButton().IsValid())
			NewTab->GetResetButton()->SetOnClicked(FOnClicked::CreateSP(this, &FMinesweeperButtonView::ResetButtonClicked));

		if (NewTab->GetField().IsValid())
		{
			NewTab->GetField()->SetOnMouseButtonDown(FPointerEventHandler::CreateSP(this, &FMinesweeperButtonView::OnMouseButtonPressed));
			NewTab->GetField()->SetOnMouseButtonUp(FPointerEventHandler::CreateSP(this, &FMinesweeperButtonView::OnMouseButtonReleased));
		}

		return NewTab;
	}
	else {
		return SNew(SDockTab);
	}
}

void FMinesweeperButtonView::InitTab()
{
	TSharedRef< FGlobalTabmanager > TabManager = FGlobalTabmanager::Get();
	Tab = StaticCastSharedPtr< SMinesweeperButtonTab >(TabManager->TryInvokeTab(SMinesweeperButtonTab::TabName));

	if (Tab.IsValid()) {
		DuplicateField();
		RenewFieldDisplay();
	}
	else {
		return;
	}
}

void FMinesweeperButtonView::OnTabClosed(TSharedRef< SDockTab > TabClosed)
{

	Destruct();
	FMinesweeperButtonModule* MinesweeperButtonModule = FMinesweeperButtonModule::GetPtr();
	MinesweeperButtonModule->ShutdownModule();

}

void FMinesweeperButtonView::SetFieldWidth(uint8 NewVal)
{
	FieldWidth = NewVal;

	if (!Tab.IsValid() || !Tab->GetWidthSlider().IsValid())
		return;

	Tab->GetWidthSlider()->SetValue(NewVal);
}

void FMinesweeperButtonView::SetFieldHeight(uint8 NewVal)
{
	FieldHeight = NewVal;

	if (!Tab.IsValid() || !Tab->GetHeightSlider().IsValid())
		return;

	Tab->GetHeightSlider()->SetValue(NewVal);
}

void FMinesweeperButtonView::SetFieldMineCount(float NewVal)
{

	FieldMineCount = NewVal;

	if (!Tab.IsValid() || !Tab->GetCountSlider().IsValid())
		return;

	Tab->GetCountSlider()->SetValue(NewVal);


}

void FMinesweeperButtonView::OnWidthSliderValueChanged(uint8 NewVal)
{
	FieldWidth = NewVal;

}

void FMinesweeperButtonView::OnHeightSliderValueChanged(uint8 NewVal)
{
	FieldHeight = NewVal;
}

void FMinesweeperButtonView::OnCountSliderValueChanged(uint16 NewVal)
{
	FieldMineCount = NewVal;
}

FReply FMinesweeperButtonView::ResetButtonClicked()
{
	FMinesweeperButtonModule* MinesweeperButtonModulePtr = FMinesweeperButtonModule::GetPtr();

	if (!MinesweeperButtonModulePtr || !MinesweeperButtonModulePtr->GetController().IsValid())
		return FReply::Handled();


	MinesweeperButtonModulePtr->GetController()->StartGame(FieldWidth, FieldHeight, FieldMineCount);

	return FReply::Handled();
}

bool FMinesweeperButtonView::DuplicateField()
{
	FMinesweeperButtonModule* MinesweeperButtonModulePtr = FMinesweeperButtonModule::GetPtr();

	if (!MinesweeperButtonModulePtr || !MinesweeperButtonModulePtr->GetModel().IsValid() || !Tab.IsValid() || !Tab->GetFieldContainer().IsValid() || !Tab->GetField().IsValid())
		return false;

	Tab->GetFieldContainer()->SetWidthOverride(MinesweeperButtonModulePtr->GetModel()->GetFieldWidth() * FMinesweeperButtonView::CellSize);
	Tab->GetFieldContainer()->SetHeightOverride(MinesweeperButtonModulePtr->GetModel()->GetFieldHeight() * FMinesweeperButtonView::CellSize);
	Tab->GetField()->ClearChildren();

	for (uint8 height_i = 0; height_i < MinesweeperButtonModulePtr->GetModel()->GetFieldHeight(); ++height_i)
	{
		for (uint8 width_i = 0; width_i < MinesweeperButtonModulePtr->GetModel()->GetFieldWidth(); ++width_i)
		{
			Tab->GetField()->AddSlot()
				.Position(FVector2D(width_i * FMinesweeperButtonView::CellSize, height_i * FMinesweeperButtonView::CellSize))
				.Size(FVector2D(FMinesweeperButtonView::CellSize, FMinesweeperButtonView::CellSize))
				[
					SNew(SMinesweeperButtonCell)
					.Model(MinesweeperButtonModulePtr->GetModel())
				.CellCoordinates({ width_i, height_i })
				];
		}
	}

	// VISUAL UPDATE ////////////////////////////////////////////////////////////////////////
	RenewFieldDisplay();
	////////////////////////////////////////////////////////////////////////////////////////

	return true;
}

Coordinates FMinesweeperButtonView::CoordinatesfromMouseEvent(const FGeometry& Geometry, const FPointerEvent& Event)
{
	FVector2D MouseLocationOnWidget = Event.GetScreenSpacePosition() - Geometry.GetAbsolutePosition();
	MouseLocationOnWidget /= Geometry.Scale;

	return { uint8(MouseLocationOnWidget.X / FMinesweeperButtonView::CellSize), uint8(MouseLocationOnWidget.Y / FMinesweeperButtonView::CellSize) };
}

FReply FMinesweeperButtonView::OnMouseButtonPressed(const FGeometry& Geometry, const FPointerEvent& Event)
{

	MouseDownKey = EKeys::Invalid;
	MouseDownKey = Event.GetEffectingButton();
	MouseDownCell = FMinesweeperButtonView::CoordinatesfromMouseEvent(Geometry, Event);


	return FReply::Handled();
}

FReply FMinesweeperButtonView::OnMouseButtonReleased(const FGeometry& Geometry, const FPointerEvent& Event)
{
	
		Coordinates MouseUpCell = FMinesweeperButtonView::CoordinatesfromMouseEvent(Geometry, Event);
		if (MouseUpCell.X == MouseDownCell.X && MouseUpCell.Y == MouseDownCell.Y)
			OnButtonClicked(Event.GetEffectingButton(), MouseUpCell);

	return FReply::Handled();
}

void FMinesweeperButtonView::OnButtonClicked(FKey MouseButton, const Coordinates& CellCoordinates)
{
	FMinesweeperButtonModule* MinesweeperButtonModulePtr = FMinesweeperButtonModule::GetPtr();

	if (!MinesweeperButtonModulePtr || !MinesweeperButtonModulePtr->GetModel().IsValid() || !MinesweeperButtonModulePtr->GetModel()->AreCellCoordsValid(CellCoordinates) || !MinesweeperButtonModulePtr->GetController().IsValid())
		return;

	if (MouseButton == EKeys::LeftMouseButton)
		MinesweeperButtonModulePtr->GetController()->LeftMouseClick(CellCoordinates);
}

void FMinesweeperButtonView::RenewFieldDisplay()
{
	if (!Tab.IsValid() || !Tab->GetField().IsValid())
		return;

	FChildren* Cells = Tab->GetField()->GetChildren();
	for (int Index = 0; Index < Cells->Num(); ++Index)
	{
		TSharedRef< SMinesweeperButtonCell > Child = StaticCastSharedRef< SMinesweeperButtonCell >(Cells->GetChildAt(Index));
		Child->UpdateCell();
	}
}

void FMinesweeperButtonView::RenewCellDisplay(const Coordinates& CellCoordinates)
{
	FMinesweeperButtonModule* MinesweeperButtonModulePtr = FMinesweeperButtonModule::GetPtr();

	if (!MinesweeperButtonModulePtr || !MinesweeperButtonModulePtr->GetModel().IsValid() || !MinesweeperButtonModulePtr->GetModel()->AreCellCoordsValid(CellCoordinates) || !Tab.IsValid() || !Tab->GetField().IsValid())
		return;

	int i = MinesweeperButtonModulePtr->GetModel()->GetIndexFromCoordinates(CellCoordinates);
	FChildren* Cells = Tab->GetField()->GetChildren();
	TSharedRef< SMinesweeperButtonCell > CellChild = StaticCastSharedRef< SMinesweeperButtonCell >(Cells->GetChildAt(i));
	CellChild->UpdateCell();
}

void FMinesweeperButtonView::TickView()
{
	if (!IsTabClosed() && Tab.IsValid())
		Tab->CreatePadding();
}

#undef LOCTEXT_NAMESPACE
