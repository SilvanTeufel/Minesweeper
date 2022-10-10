// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TickableEditorObject.h"
#include "Containers/Queue.h"
#include "Data.h"

class MinesweeperButtonModule;
struct Coordinates;

class FMinesweeperButtonController : FTickableEditorObject
{
public:

	FMinesweeperButtonController();
	void Init();
	MB_GameState GetGameStatus() const { return GameStatus; }

	void InitGame();
	bool StartGame(uint8 Width, uint8 Height, uint16 MineCount);

	// USED BY VIEW WHEN CELLS GET CLICKED ///////////////////////////
	void LeftMouseClick(const Coordinates& CellCoordinates);
	/////////////////////////////////////////////////////////////////////

	// IS TRUE WHEN CONTROLLER IS REVEALING CELLS ///////////////////////
	bool CellsGettingRevealed() const;
	/////////////////////////////////////////////////////////////////////

	 /////// OVERRIDES ///////////////////////////////////////////////////
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Always; }
	virtual bool IsTickable() const override { return true; }
	virtual TStatId GetStatId() const override; // This is needed, but i dont get why :-) -> Programm crash without.
	virtual void Tick(float DeltaTime) override;
	/////////////////////////////////////////////////////////////////////


protected:

	// CONTROLLING ///////////////////////////////////////////////////	
	static TArray< bool > CreateRandomMines(uint16 GridSize, uint16 Count);
	MB_GameState GameStatus;
	TQueue< Coordinates > CellsToReveal;
	float RevealTickTime;
	void EndGame(MB_GameState);
	/////////////////////////////////////////////////////////////////////

	// NO CONTROLLING AVAILABE ANYMORE
	void ControlError();
	/////////////////////////////////////////////////////////////////////

	// CALLED BY TICK TO REVEAL CELLS
	void RevealTick(const Coordinates& CellCoordinates);
	/////////////////////////////////////////////////////////////////////

};
