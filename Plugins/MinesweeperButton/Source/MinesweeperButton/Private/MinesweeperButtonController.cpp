
#include "MinesweeperButtonController.h"
#include "MinesweeperButtonModule.h"
#include "UObject/Object.h"
#include "MinesweeperButtonView.h"


FMinesweeperButtonController::FMinesweeperButtonController(){
	GameStatus = Stop;
	CellsToReveal.Empty();
	RevealTickTime = 0.f;
}

void FMinesweeperButtonController::Init()
{
	FMinesweeperButtonModule* MinesweeperButtonModulePtr = FMinesweeperButtonModule::GetPtr();

	if (!MinesweeperButtonModulePtr ||
		!MinesweeperButtonModulePtr->GetModel().IsValid() ||
		!MinesweeperButtonModulePtr->GetView().IsValid())
	{
		ControlError();
		return;
	}

	InitGame();
}

void FMinesweeperButtonController::InitGame()
{
	FMinesweeperButtonModule* MinesweeperButtonModulePtr = FMinesweeperButtonModule::GetPtr();

	if (!MinesweeperButtonModulePtr || !MinesweeperButtonModulePtr->GetModel().IsValid() || !MinesweeperButtonModulePtr->GetView().IsValid())
		return;

	if (GameStatus == Stop)
	{
		MinesweeperButtonModulePtr->GetView()->SetFieldWidth(MB_Field_width);
		MinesweeperButtonModulePtr->GetView()->SetFieldHeight(MB_Field_height);
		MinesweeperButtonModulePtr->GetView()->SetFieldMineCount(MB_Field_Minecount);

		StartGame(
			MinesweeperButtonModulePtr->GetView()->GetFieldWidth(),
			MinesweeperButtonModulePtr->GetView()->GetFieldHeight(),
			MinesweeperButtonModulePtr->GetView()->GetMineCount()
		);
	}

	MinesweeperButtonModulePtr->GetView()->InitTab();
}

bool FMinesweeperButtonController::StartGame(uint8 Width, uint8 Height, uint16 MineCount)
{
	if (GameStatus == Error)
		return false;

	FMinesweeperButtonModule* MinesweeperButtonModulePtr = FMinesweeperButtonModule::GetPtr();

	if (!MinesweeperButtonModulePtr ||
		!MinesweeperButtonModulePtr->GetModel().IsValid() ||
		!MinesweeperButtonModulePtr->GetView().IsValid())
		return false;

	TArray< bool > Mines = CreateRandomMines(
		Width * Height,
		MineCount
	);

	CellsToReveal.Empty();
	MinesweeperButtonModulePtr->GetModel()->InitField(Width, Height, Mines);
	MinesweeperButtonModulePtr->GetView()->DuplicateField();
	GameStatus = Gaming;

	return true;
}


void FMinesweeperButtonController::LeftMouseClick(const Coordinates& CellCoordinates)
{
	if (GameStatus != Gaming || CellsGettingRevealed())
		return;

	FMinesweeperButtonModule* MinesweeperButtonModulePtr = FMinesweeperButtonModule::GetPtr();

	if (!MinesweeperButtonModulePtr || !MinesweeperButtonModulePtr->GetModel().IsValid() || !MinesweeperButtonModulePtr->GetView().IsValid())
	{
		ControlError();
		return;
	}

	if (!MinesweeperButtonModulePtr->GetModel()->AreCellCoordsValid(CellCoordinates))
		return;

	const CellData& CellInfo = MinesweeperButtonModulePtr->GetModel()->CellCoordinatesToInfo(CellCoordinates);

	if (CellInfo.bIsMine)
	{
		MinesweeperButtonModulePtr->GetModel()->ExplodeCell(CellCoordinates);
		EndGame(Lose);
		return;
	}

	RevealTickTime = 0.f;
	RevealTick(CellCoordinates);
}

bool FMinesweeperButtonController::CellsGettingRevealed() const
{
	return !CellsToReveal.IsEmpty();
};

void FMinesweeperButtonController::RevealTick(const Coordinates& CellCoordinates)
{
	FMinesweeperButtonModule* MinesweeperButtonModulePtr = FMinesweeperButtonModule::GetPtr();

	if (!MinesweeperButtonModulePtr || !MinesweeperButtonModulePtr->GetModel().IsValid() || !MinesweeperButtonModulePtr->GetView().IsValid())
	{
		ControlError();
		return;
	}

	if (!MinesweeperButtonModulePtr->GetModel()->AreCellCoordsValid(CellCoordinates))
		return;

	const CellData& CellInfo = MinesweeperButtonModulePtr->GetModel()->CellCoordinatesToInfo(CellCoordinates);

	// Is Cell is a Mine or ifs this Cell Allready Revelead /////////////////////////////////////////
	if (CellInfo.bIsMine || !MinesweeperButtonModulePtr->GetModel()->RevealCell(CellCoordinates))
		return;

	// Update the Cell
	MinesweeperButtonModulePtr->GetView()->RenewCellDisplay(CellCoordinates);

	// MineCount + CellsReaveled >= Field
	if ((MinesweeperButtonModulePtr->GetModel()->GetMineCount() + MinesweeperButtonModulePtr->GetModel()->GetRevealedCellCount()) >= MinesweeperButtonModulePtr->GetModel()->GetFieldSize())
	{
		EndGame(Win);
		return;
	}

	// If cell is nerby mine, dont reveal
	if (MinesweeperButtonModulePtr->GetModel()->GetSurroundingMineCount(CellCoordinates) > 0)
		return;

	// Enqueue new cells to reveal
	Coordinates CellToAdd;

	// If cell is top
	if (CellCoordinates.Y > 0)
	{
		CellToAdd = CellCoordinates;
		CellToAdd.Y--;
		CellsToReveal.Enqueue(CellToAdd);
	}

	// If cell is left
	if (CellCoordinates.X > 0)
	{
		CellToAdd = CellCoordinates;
		CellToAdd.X--;
		CellsToReveal.Enqueue(CellToAdd);
	}

	// If cell is bottom
	if (CellCoordinates.Y < (MinesweeperButtonModulePtr->GetModel()->GetFieldHeight() - 1))
	{
		CellToAdd = CellCoordinates;
		CellToAdd.Y++;
		CellsToReveal.Enqueue(CellToAdd);
	}

	// If cell is right
	if (CellCoordinates.X < (MinesweeperButtonModulePtr->GetModel()->GetFieldWidth() - 1))
	{
		CellToAdd = CellCoordinates;
		CellToAdd.X++;
		CellsToReveal.Enqueue(CellToAdd);
	}

	// If cell is top left
	if (CellCoordinates.X > 0 && CellCoordinates.Y > 0)
	{
		CellToAdd = CellCoordinates;
		CellToAdd.X--;
		CellToAdd.Y--;
		CellsToReveal.Enqueue(CellToAdd);
	}

	// If cell is top right
	if (CellCoordinates.X < (MinesweeperButtonModulePtr->GetModel()->GetFieldWidth() - 1) && CellCoordinates.Y > 0)
	{
		CellToAdd = CellCoordinates;
		CellToAdd.X++;
		CellToAdd.Y--;
		CellsToReveal.Enqueue(CellToAdd);
	}

	// If cell is bottom left
	if (CellCoordinates.X > 0 && CellCoordinates.Y < (MinesweeperButtonModulePtr->GetModel()->GetFieldHeight() - 1))
	{
		CellToAdd = CellCoordinates;
		CellToAdd.X--;
		CellToAdd.Y++;
		CellsToReveal.Enqueue(CellToAdd);
	}

	// If cell is bottom right
	if (CellCoordinates.X < (MinesweeperButtonModulePtr->GetModel()->GetFieldWidth() - 1) && CellCoordinates.Y < (MinesweeperButtonModulePtr->GetModel()->GetFieldHeight() - 1))
	{
		CellToAdd = CellCoordinates;
		CellToAdd.X++;
		CellToAdd.Y++;
		CellsToReveal.Enqueue(CellToAdd);
	}
}

void FMinesweeperButtonController::EndGame(MB_GameState NewStatus)
{
	if (GameStatus != Gaming)
		return;

	CellsToReveal.Empty();

	FMinesweeperButtonModule* MinesweeperButtonPtr = FMinesweeperButtonModule::GetPtr();

	if (!MinesweeperButtonPtr || !MinesweeperButtonPtr->GetModel().IsValid() || !MinesweeperButtonPtr->GetView().IsValid())
	{
		ControlError();
		return;
	}

	MinesweeperButtonPtr->GetModel()->RevealField();
	GameStatus = NewStatus;
	MinesweeperButtonPtr->GetView()->RenewFieldDisplay();
}

void FMinesweeperButtonController::ControlError()
{
	CellsToReveal.Empty();
	GameStatus = Error;
}

TStatId FMinesweeperButtonController::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(FMinesweeperButtonController, STATGROUP_Tickables);
}

void FMinesweeperButtonController::Tick(float DeltaTime)
{
	FMinesweeperButtonModule* MinesweeperModulePtr = FMinesweeperButtonModule::GetPtr();

	if (MinesweeperModulePtr && MinesweeperModulePtr->GetView().IsValid())
	{
			MinesweeperModulePtr->GetView()->TickView();
	}

	if (GameStatus != Gaming || CellsToReveal.IsEmpty()) {
		RevealTickTime = 0.f;
		return;
	}

	static const float TimePerReveal = 0.0025f;
	RevealTickTime += DeltaTime;

	while (RevealTickTime > TimePerReveal)
	{
		Coordinates NextLocation;
		CellsToReveal.Dequeue(NextLocation);
		RevealTick(NextLocation);

		if (CellsToReveal.IsEmpty())
			RevealTickTime = 0.f;
	

		RevealTickTime -= TimePerReveal;
	}
}


TArray< bool > FMinesweeperButtonController::CreateRandomMines(uint16 GridSize, uint16 Count)
{

	TArray< bool > Mines;
	Mines.SetNum(GridSize);

	
	for (int i = 0; i < Count; ++i)
	{
		// m_i is the index of the mine
		uint16 m_i = (uint16)round(GridSize * (((float)rand()) / ((float)RAND_MAX))) - 1;
		Mines[m_i] && m_i>=0 ? i-- : Mines[m_i] = true;
	}

	return Mines;
}
