// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Widget/StageResultWindow.h"
#include "Components/Button.h"
#include "ProjectVSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/RichTextBlock.h"
#include "PlayGameState.h"
#include "ProjectVS.h"

void UStageResultWindow::NativeConstruct()
{
	Super::NativeConstruct();

	ToTitleBTN->OnClicked.AddDynamic(this, &UStageResultWindow::OnClickedToTileBTN);
}

void UStageResultWindow::NativeDestruct()
{
	LOG_ERROR(TEXT("Call NativeDestruct"));

	Super::NativeDestruct();
}

void UStageResultWindow::OnClickedToTileBTN()
{
	AProjectVSPlayerController* PlayerController = GetOwningPlayer<AProjectVSPlayerController>();
	if(PlayerController == nullptr) return;

	UGameplayStatics::OpenLevel(PlayerController, "TitleLevel");
}

void UStageResultWindow::SetStageResult(bool bCleared)
{
	const FText& NewText = bCleared ? ClearText : FailText;

	StageResultText->SetText(NewText);
}

void UStageResultWindow::SetResultGold(float NewGold)
{
	ResultGoldText->SetText(FText::FromString(FString::Printf(TEXT("%.0f G"), NewGold)));
}

void UStageResultWindow::SetStageTime(float NewTime)
{
}

void UStageResultWindow::SetMonsterKillCount(int NewCount)
{
}

void UStageResultWindow::UpdateStageResultWindow()
{
	UWorld* CurrentWorld = GetWorld();
	if(CurrentWorld == nullptr) return;

	APlayGameState* PlayGameState = CurrentWorld->GetGameState<APlayGameState>();
	if(PlayGameState == nullptr) return;

	bool bCleared = PlayGameState->GetStageState() == EStageState::Clear ? true : false;
	SetStageResult(bCleared);

	float FinalStageTime = PlayGameState->GetStageTime();
	SetStageTime(FinalStageTime);

	int FinalMonsterKillCount = PlayGameState->GetMonsterKillCount();
	SetMonsterKillCount(FinalMonsterKillCount);



}
