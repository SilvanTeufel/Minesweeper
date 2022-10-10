// Copyright 

#pragma once

#include "CoreMinimal.h"
#include "Data.h"


struct CellData
{
	bool bIsMine;
	CellState State;
};


struct Coordinates
{
	uint8 X;
	uint8 Y;
};

class FMinesweeperButtonModel
{

public:

	static const CellData GeneralCellData;

	FMinesweeperButtonModel();

	// FIELD INFORMATIONS /////////////////////////////////////////////////////////////
	uint16 GetFieldSize() const { return (uint16)Field.Num(); }
	uint8 GetFieldWidth() const { return FieldWidth; }
	uint8 GetFieldHeight() const { return FieldHeight; }
	uint16 GetMineCount() const { return MineCount; }
	uint16 GetRevealedCellCount() const { return RevealedCellCount; }
	/////////////////////////////////////////////////////////////////////////////////////

	// INIT A Field ///////////////////////////////////////////////////////////////
	void InitField(uint8 InWidth, uint8 InHeight, TArray< bool > MineLocations);
	/////////////////////////////////////////////////////////////////////////////////////


	// CHANGE MODEL VIA INDEXING ///////////////////////////////////////////////////////////////
	bool RevealCell(const Coordinates& CellCoordinates);
	bool ExplodeCell(const Coordinates& CellCoordinates);
	void RevealField();

	Coordinates GetCoordinatesfromIndex(uint16 Index) const;
	uint16 GetIndexFromPoints(uint8 X, uint8 Y) const;
	uint16 GetIndexFromCoordinates(const Coordinates& CellCoordinates) const;
	bool AreCellCoordsValid(const Coordinates& CellCoordinates) const;
	bool ArePointsValid(uint8 X, uint8 Y) const;
	const CellData& PointsToCellData(uint8 X, uint8 Y) const;
	const CellData& CellCoordinatesToInfo(const Coordinates& CellCoordinates) const;
	uint16 GetSurroundingMineCount(const Coordinates& CellCoordinates) const;
	//////////////////////////////////////////////////////////////////


protected:

	TArray< CellData > Field;
	uint8 FieldWidth;
	uint8 FieldHeight;
	uint16 MineCount;
	uint16 RevealedCellCount;

};