

#include "MinesweeperButtonModule.h"
#include "MinesweeperButtonStyle.h"
#include "MinesweeperButtonCommands.h"

#include "MinesweeperButtonModel.h"
#include "MinesweeperButtonView.h"
#include "MinesweeperButtonController.h"


#include "Misc/MessageDialog.h"
#include "ToolMenus.h"

static const FName MinesweeperButtonTabName("MinesweeperButton");

#define LOCTEXT_NAMESPACE "FMinesweeperButtonModule"



void FMinesweeperButtonModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FMinesweeperButtonStyle::Load();
	FMinesweeperButtonCommands::Register();
}

void FMinesweeperButtonModule::ShutdownModule()
{
	WindowClosed = true;
}

FMinesweeperButtonModule& FMinesweeperButtonModule::Get()
{
	return FModuleManager::GetModuleChecked< FMinesweeperButtonModule >("MinesweeperButton");
}

FMinesweeperButtonModule* FMinesweeperButtonModule::GetPtr()
{
	return static_cast<FMinesweeperButtonModule*>(FModuleManager::Get().GetModule("MinesweeperButton"));
}


bool FMinesweeperButtonModule::IsButtonClickable()
{
	return true;
}

void FMinesweeperButtonModule::ButtonClicked()
{

	FMinesweeperButtonModule& MinesweeperButtonModulePtr = FMinesweeperButtonModule::Get();

	if (MinesweeperButtonModulePtr.WindowClosed)
	{
		MinesweeperButtonModulePtr.MinesweeperButtonModel = MakeShared< FMinesweeperButtonModel >();
		MinesweeperButtonModulePtr.MinesweeperButtonView = MakeShared< FMinesweeperButtonView >();
		MinesweeperButtonModulePtr.MinesweeperButtonController = MakeShared< FMinesweeperButtonController >();
		MinesweeperButtonModulePtr.MinesweeperButtonView->Init();
		MinesweeperButtonModulePtr.MinesweeperButtonController->Init();
		MinesweeperButtonModulePtr.WindowClosed = false;
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinesweeperButtonModule, MinesweeperButton)