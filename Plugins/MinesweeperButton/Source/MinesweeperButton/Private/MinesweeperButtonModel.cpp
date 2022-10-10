
#include "MinesweeperButtonModel.h"
#include "MinesweeperButtonModule.h"

const CellData FMinesweeperButtonModel::GeneralCellData = { false, Hide };

FMinesweeperButtonModel::FMinesweeperButtonModel(){
	Field.SetNum(0);
	FieldWidth = 0;
	FieldHeight = 0;
	MineCount = 0;
	RevealedCellCount = 0;
}


void FMinesweeperButtonModel::InitField(uint8 InWidth, uint8 InHeight, TArray< bool > MineLocations)
{
	RevealedCellCount = 0;


	FieldWidth = InWidth > 0 ? InWidth : 1;
	FieldHeight = InHeight > 0 ? InHeight : 1;
	// RESIZING ARRAY		/// FIELD WIDTH /////			//// FIELD HEIGHT //////
	Field.SetNum(((uint16)FieldWidth) * ((uint16)FieldHeight));
	MineCount = 0;

	for (int i = 0; i < GetFieldSize(); ++i)
	{
		Field[i] = FMinesweeperButtonModel::GeneralCellData;
		if (i < MineLocations.Num())
		{
			Field[i].bIsMine = MineLocations[i];
			if (MineLocations[i])
				++MineCount;
		}
	}

	// Must be at least 1 mine
	if (MineCount == 0)
	{
		MineCount = 1;
		Field[0].bIsMine = true;
	}
}




Coordinates FMinesweeperButtonModel::GetCoordinatesfromIndex(uint16 Index) const
{
	return {
		(uint8)(Index % FieldWidth),
		(uint8)(Index / ((uint16)FieldWidth))
	};
}


uint16 FMinesweeperButtonModel::GetIndexFromPoints(uint8 X, uint8 Y) const
{
	return (
		((uint16)X) +
		((uint16)FieldWidth) * ((uint16)Y)
		);
}

uint16 FMinesweeperButtonModel::GetIndexFromCoordinates(const Coordinates& CellCoordinates) const
{
	return GetIndexFromPoints(CellCoordinates.X, CellCoordinates.Y);
}

bool FMinesweeperButtonModel::ArePointsValid(uint8 X, uint8 Y) const
{
	return (X < FieldWidth&& Y < FieldHeight);
}

bool FMinesweeperButtonModel::AreCellCoordsValid(const Coordinates& CellCoordinates) const
{ 
	return ArePointsValid(CellCoordinates.X, CellCoordinates.Y);
};

const CellData& FMinesweeperButtonModel::PointsToCellData(uint8 X, uint8 Y) const
{
	if (!ArePointsValid(X, Y))
		return FMinesweeperButtonModel::GeneralCellData;

	uint16 i = GetIndexFromPoints(X, Y);

	if (i >= GetFieldSize())
		return FMinesweeperButtonModel::GeneralCellData;

	return Field[i];
}

const CellData& FMinesweeperButtonModel::CellCoordinatesToInfo(const Coordinates& CellCoordinates) const
{ 
	return PointsToCellData(CellCoordinates.X, CellCoordinates.Y);
};

uint16 FMinesweeperButtonModel::GetSurroundingMineCount(const Coordinates& CellCoordinates) const
{
	if (!AreCellCoordsValid(CellCoordinates))
		return 0;

	uint16 SurroundingMineCount = 0;

	// If cell is top left
	if ((CellCoordinates.X > 0 && CellCoordinates.Y > 0) &&
		(PointsToCellData(CellCoordinates.X - 1, CellCoordinates.Y - 1).bIsMine))
			++SurroundingMineCount;

	// If cell is top 
	if ((CellCoordinates.Y > 0) &&
		PointsToCellData(CellCoordinates.X, CellCoordinates.Y - 1).bIsMine)
			++SurroundingMineCount;

	//  If cell is top right
	if ((CellCoordinates.X < (FieldWidth - 1) && CellCoordinates.Y > 0) &&
		PointsToCellData(CellCoordinates.X + 1, CellCoordinates.Y - 1).bIsMine)
			++SurroundingMineCount;

	// If cell is left
	if ((CellCoordinates.X > 0) &&
		PointsToCellData(CellCoordinates.X - 1, CellCoordinates.Y).bIsMine)
			++SurroundingMineCount;

	// If cell is right
	if ((CellCoordinates.X < (FieldWidth - 1)) &&
		PointsToCellData(CellCoordinates.X + 1, CellCoordinates.Y).bIsMine)
			++SurroundingMineCount;
	
	// If cell is bottom left
	if ((CellCoordinates.X > 0 && CellCoordinates.Y < (FieldHeight - 1)) &&
		PointsToCellData(CellCoordinates.X - 1, CellCoordinates.Y + 1).bIsMine)
			++SurroundingMineCount;

	// If cell is bottom
	if ((CellCoordinates.Y < (FieldHeight - 1)) &&
		PointsToCellData(CellCoordinates.X, CellCoordinates.Y + 1).bIsMine)
			++SurroundingMineCount;

	// If cell is bottom right
	if ((CellCoordinates.X < (FieldWidth - 1) && CellCoordinates.Y < (FieldHeight - 1)) &&
		PointsToCellData(CellCoordinates.X + 1, CellCoordinates.Y + 1).bIsMine)
			++SurroundingMineCount;

	

	return SurroundingMineCount;
}

bool FMinesweeperButtonModel::RevealCell(const Coordinates& CellCoordinates)
{
	if (!AreCellCoordsValid(CellCoordinates))
		return false;

	uint16 i = GetIndexFromCoordinates(CellCoordinates);

	if (i >= GetFieldSize())
		return false;

	if (Field[i].State == Reveal)
		return false;

	Field[i].State = Reveal;

	if (!Field[i].bIsMine)
		++RevealedCellCount;

	return true;
}

bool FMinesweeperButtonModel::ExplodeCell(const Coordinates& CellCoordinates)
{
	if (!AreCellCoordsValid(CellCoordinates))
		return false;

	uint16 i = GetIndexFromCoordinates(CellCoordinates);

	if (i >= GetFieldSize() || Field[i].State == Explode)
		return false;

	Field[i].State = Explode;
	return true;
}

void FMinesweeperButtonModel::RevealField()
{
	for (int i = 0; i < GetFieldSize(); ++i)
	{
		if (Field[i].State != Explode)
			Field[i].State = Reveal;
	}

	RevealedCellCount = (uint16)(GetFieldSize() - MineCount);
}
