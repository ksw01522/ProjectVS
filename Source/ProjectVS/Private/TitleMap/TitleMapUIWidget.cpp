// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleMap/TitleMapUIWidget.h"
#include "VSGameInstance.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "TitleMap/UpgradeWindowWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/RichTextBlock.h"
#include "VSGameInstance.h"

#define LOCTEXT_NAMESPACE "ProjectVSWidget"



void UTitleMapUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UVSGameInstance* VSGameInst = GetGameInstance<UVSGameInstance>();

	int CurrentGold = VSGameInst->GetGold();

	GoldTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentGold)));

	OnGoldChangedHandle = VSGameInst->GetOnGoldChangedDelegate().AddUObject(this, &UTitleMapUIWidget::OnGoldChanged);

	StartPlayButton->OnClicked.AddDynamic(this, &UTitleMapUIWidget::StartPlayGame);
	
	OpenUpgradeButton->OnClicked.AddDynamic(this, &UTitleMapUIWidget::OpenUpgradeWindow);

	QuitButton->OnClicked.AddDynamic(this, &UTitleMapUIWidget::QuitGame);


}

void UTitleMapUIWidget::NativeDestruct()
{
	if (UVSGameInstance* VSGameInst = GetGameInstance<UVSGameInstance>())
	{
		VSGameInst->GetOnGoldChangedDelegate().Remove(OnGoldChangedHandle);
	}

	Super::NativeDestruct();
}

void UTitleMapUIWidget::StartPlayGame()
{
	UVSGameInstance* VSGameInst = GetGameInstance<UVSGameInstance>();
	if (VSGameInst)
	{
		VSGameInst->LoadPlayMap();
	}
}

void UTitleMapUIWidget::OpenUpgradeWindow()
{
	UpgradeWindow->SetVisibility(ESlateVisibility::Visible);
}

void UTitleMapUIWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}

void UTitleMapUIWidget::OnGoldChanged(int PrevGold, int NewGold)
{
	int CurrentGold = NewGold;

	GoldTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentGold)));
}

#undef LOCTEXT_NAMESPACE