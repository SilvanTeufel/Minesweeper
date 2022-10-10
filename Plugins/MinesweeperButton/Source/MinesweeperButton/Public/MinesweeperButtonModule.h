// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMinesweeperButtonModel;
class FMinesweeperButtonView;
class FMinesweeperButtonController;

class FToolBarBuilder;
class FMenuBuilder;

class FMinesweeperButtonModule : public IModuleInterface
{
public:

	static FMinesweeperButtonModule& Get();
	static FMinesweeperButtonModule* GetPtr();

	// LOADS/UNLOADS STYLE // REGISTERS/UNREGISTERS COMMAND  /////////////////
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	//////////////////////////////////////////////////////////////////////////
	
	//  THIS FUNCTIONS ARE BOUND TO COMMAND //////////////////////////////////
	static void ButtonClicked();
	static bool IsButtonClickable();
	//////////////////////////////////////////////////////////////////////////

	bool WindowClosed = true;

	TSharedPtr < FMinesweeperButtonModel > GetModel() const { return MinesweeperButtonModel; };
	TSharedPtr < FMinesweeperButtonView > GetView() const { return MinesweeperButtonView; };
	TSharedPtr < FMinesweeperButtonController > GetController() const { return MinesweeperButtonController; };

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TSharedPtr< FMinesweeperButtonModel > MinesweeperButtonModel;
	TSharedPtr< FMinesweeperButtonView > MinesweeperButtonView;
	TSharedPtr< FMinesweeperButtonController > MinesweeperButtonController;

};
