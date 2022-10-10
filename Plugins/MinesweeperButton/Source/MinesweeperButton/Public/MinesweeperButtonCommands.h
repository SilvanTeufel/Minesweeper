// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "MinesweeperButtonStyle.h"

class FMinesweeperButtonCommands : public TCommands<FMinesweeperButtonCommands>
{
public:

	FMinesweeperButtonCommands();

	virtual void RegisterCommands() override;
	void CreateCommandList();
public:
	TSharedPtr< FUICommandList > CommandList;
	TSharedPtr< FUICommandInfo > CommandInfo;
};
