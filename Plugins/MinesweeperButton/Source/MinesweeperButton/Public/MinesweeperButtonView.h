
#pragma once

#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "MinesweeperButtonModel.h"



class FMinesweeperButtonModule;
class FReply;
class SMinesweeperButtonTab;
class SDockTab;
class FSpawnTabArgs;
struct FGeometry;
struct FPointerEvent;

class FMinesweeperButtonView : public TSharedFromThis< FMinesweeperButtonView >
{

public:

	// Width and height of the Field cells (in slate units.)
	static const float CellSize;

	FMinesweeperButtonView();


	void Init(); // Registers Tabspawner
	void Destruct(); // Unregisters Tabspawner

	void InitTab(); // Duplicates a Field and Renews the Field Display and Invokes Tab
	bool IsTabClosed() const { return !Tab.IsValid(); }
	void OnTabClosed(TSharedRef< SDockTab > TabClosed);

	// USED IN CONTROLLER /////////////////////////////////
	uint8 GetFieldWidth() const { return FieldWidth; };
	uint8 GetFieldHeight() const { return FieldHeight; };
	uint16 GetMineCount() const { return FieldMineCount; };
	uint16 GetFieldMineMaxCount() const { return FieldMaxMineCount; };
	void SetFieldWidth(uint8 NewVal);
	void SetFieldHeight(uint8 NewVal);
	void SetFieldMineCount(float NewVal);

	// UI CALLBACKS USED IN TAB///////////////////////////////////////////
	void OnWidthSliderValueChanged(uint8 NewVal);
	void OnHeightSliderValueChanged(uint8 NewVal);
	void OnCountSliderValueChanged(uint16 NewVal);
	/////////////////////////////////////////////////////////////////////


	/// CREATES NEW FIELD AND THROWS CURRENT FIELD /// BASED ON MODEL ///
	bool DuplicateField();
	/////////////////////////////////////////////////////////////////////

	/// REWNEW A CELLL /////////////////////////////////////
	void RenewCellDisplay(const Coordinates& CellCoordinates);
	/////////////////////////////////////////////////////////////////////


	// RENEW THE FIELDDISPLAY /////////////////////////////////////////
	void RenewFieldDisplay();
	/////////////////////////////////////////////////////////////////////

	void TickView(); // Is just creating the Padding


protected:


	// CREATE TAB ////////////////////////////////////////////////////
	TSharedRef< SDockTab > CreateTab(const FSpawnTabArgs& TabSpawnArgs);
	//////////////////////////////////////////////////////////////////////////////

	// UI/MOUSE INTERACTION ////////////////////////////////////////////////////
	FReply ResetButtonClicked();
	FReply OnMouseButtonPressed(const FGeometry& Geometry, const FPointerEvent& Event);
	FReply OnMouseButtonReleased(const FGeometry& Geometry, const FPointerEvent& Event);
	void OnButtonClicked(FKey MouseButton, const Coordinates& Cell);
	static Coordinates CoordinatesfromMouseEvent(const FGeometry& Geometry, const FPointerEvent& Event);
	//////////////////////////////////////////////////////////////////////////////

	TSharedPtr< SMinesweeperButtonTab > Tab;

	FKey MouseDownKey;
	Coordinates MouseDownCell;

	// CURRENT SLIDER VALUES ////////////////////////////////////////////////////
	uint8 FieldWidth;
	uint8 FieldHeight;
	uint16 FieldMineCount;
	uint16 FieldMaxMineCount;
	//////////////////////////////////////////////////////////////////////////////


};
