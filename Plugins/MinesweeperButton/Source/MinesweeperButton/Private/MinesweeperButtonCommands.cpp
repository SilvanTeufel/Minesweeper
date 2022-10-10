// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "MinesweeperButtonCommands.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "MinesweeperButtonModule.h"
#include "LevelEditor.h"
#include "Styling/SlateStyle.h"
#include "ToolMenus.h"
#include "Data.h"


#define LOCTEXT_NAMESPACE "FMinesweeperButtonModule"

FMinesweeperButtonCommands::FMinesweeperButtonCommands()
	: TCommands<FMinesweeperButtonCommands>(TEXT("MinesweeperButton"),
		NSLOCTEXT("Contexts", "MinesweeperButton", "MinesweeperButton Plugin"),
		NAME_None,
		FMinesweeperButtonStyle::Get()->GetStyleSetName())
{
}

void FMinesweeperButtonCommands::CreateCommandList()
{
	// Create command list and add it to the editor
	CommandList = MakeShareable(new FUICommandList);
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
	CommandList->Append(LevelEditorModule.GetGlobalLevelEditorActions());

	CommandList = MakeShareable(new FUICommandList);
	CommandList->MapAction(
		CommandInfo,
		FExecuteAction::CreateStatic(&FMinesweeperButtonModule::ButtonClicked), // OnToolbarButtonClicked is Call
		FCanExecuteAction::CreateStatic(&FMinesweeperButtonModule::IsButtonClickable) // Check if Button is Clickable
	);

}

void FMinesweeperButtonCommands::RegisterCommands()
{
	// Only Proceed when CommandList is undefined
	if (CommandList.IsValid())
		return;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Create UI Command  //////////////////////////////////////////////////////////////////////////////////
	UI_COMMAND(CommandInfo, "MinesweeperButton", "Dont hesitate and play Minesweeper", EUserInterfaceActionType::Button, FInputChord());// FInputGesture()
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Create command list and add it to the editor
	CreateCommandList();

	// Use Owner for cleanup -> UToolMenus::UnregisterOwner /////////////////////////////////////////////////
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FText InLabel = FText::FromString(TEXT("Minesweeper"));
				// Add the Button with CommandInfo and set CommandList //////////////////////////////////
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(CommandInfo, InLabel));
				Entry.SetCommandList(CommandList);
				////////////////////////////////////////////////////////////////////////////////////////
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////
}

#undef LOCTEXT_NAMESPACE
