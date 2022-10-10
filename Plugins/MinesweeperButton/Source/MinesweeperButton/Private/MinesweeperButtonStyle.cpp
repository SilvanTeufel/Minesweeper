// Copyright 2022 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "MinesweeperButtonStyle.h"
#include "MinesweeperButtonModule.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"
#include "Engine/Texture2D.h"
#include "Styling/SlateStyle.h"
#include "Data.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr< class FSlateStyleSet> FMinesweeperButtonStyle::Style = nullptr;

void FMinesweeperButtonStyle::Load()
{
	CreateStyle();
}

void FMinesweeperButtonStyle::Unload()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*Style.Get());
	ensure(Style.IsUnique());
	Style.Reset();
}

FName FMinesweeperButtonStyle::GetStyleSetName()
{
	static FName StyleSetName = MB_StyleName;
	return StyleSetName;
}


void FMinesweeperButtonStyle::CreateStyle()
{

	if (Style.IsValid())
		return;

	Style = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("MinesweeperButton")->GetBaseDir() / TEXT("Resources"));
	
	FSlateStyleRegistry::RegisterSlateStyle(*Style.Get());

	if (!Style.IsValid()) { return; }

	static UTexture2D* ButtonIcon = LoadObject< UTexture2D >(nullptr, Path_Button_Icon);
	check(ButtonIcon);

	static UTexture2D* MineIcon = LoadObject< UTexture2D >(nullptr, Path_Mine_Icon);
	check(MineIcon);

	FLinearColor DefaultColor = FLinearColor(0.0f, 0.0f, 1.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);


	Style->Set("MinesweeperButton.CommandInfo", new IMAGE_BRUSH_SVG(TEXT("PlaceholderButtonIcon"), Icon20x20));
	Style->Set(MB_ButtonStyle, new FSlateImageBrush(ButtonIcon, FVector2D(64.f, 64.f)));
	Style->Set(MB_MineStyle, new FSlateImageBrush(MineIcon, FVector2D(32.f, 32.f)));
	Style->Set(MB_BackgroundStyle, FLinearColor(0.5f, 0.5f, 0.5f, 0.5f)); 
	Style->Set(MB_ExpBackgroundStyle, FLinearColor(1.0f, 0.25f, 0.25f)); 
	Style->Set(MB_MenuStyle, FCoreStyle::GetDefaultFontStyle("Regular", 12));
	Style->Set(MB_NextMineStyle, FCoreStyle::GetDefaultFontStyle("Regular", 12));
	Style->Set(MB_NextMineTextStyle, DefaultColor);


}

TSharedPtr< class FSlateStyleSet > FMinesweeperButtonStyle::Get()
{
	return Style;
}

