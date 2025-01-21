// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Widget/StageResultWindow.h"
#include "Components/Button.h"
#include "ProjectVSPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/RichTextBlock.h"
#include "PlayGameState.h"
#include "ProjectVS.h"
#include "Player/VSPlayerState.h"
#include "VSGameInstance.h"

void UStageResultWindow::NativeConstruct()
{
	Super::NativeConstruct();

	ToTitleBTN->OnClicked.AddDynamic(this, &UStageResultWindow::OnClickedToTileBTN);
}

void UStageResultWindow::NativeDestruct()
{
	Super::NativeDestruct();
}

void UStageResultWindow::OnClickedToTileBTN()
{
	AProjectVSPlayerController* PlayerController = GetOwningPlayer<AProjectVSPlayerController>();
	if(PlayerController == nullptr) return;

	AVSPlayerState* PlayerState = GetOwningPlayerState<AVSPlayerState>();
	PlayerState->SaveGold();

	UVSGameInstance* VSGameInst = GetGameInstance<UVSGameInstance>();
	VSGameInst->LoadTitleMap();
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
	FFormatNamedArguments Args;

	int NewMinute = NewTime / 60;
	int NewSeconds = NewTime - (NewMinute * 60);

	FString NewMinuteString = NewMinute < 10 ? FString::Printf(TEXT("0%d"), NewMinute) : FString::FromInt(NewMinute);
	FString NewSecondsString = NewSeconds < 10 ? FString::Printf(TEXT("0%d"), NewSeconds) : FString::FromInt(NewSeconds);

	FString NewStageTime = FString::Printf(TEXT("%s : %s"), *NewMinuteString, *NewSecondsString);
	StageTimeText->SetText(FText::FromString(NewStageTime));
}

void UStageResultWindow::SetMonsterKillCount(int NewCount)
{
	FFormatNamedArguments Args;

	Args.Add("KillCount", NewCount);

	KillCountText->SetText(FText::Format(NSLOCTEXT("VSWidget", "Result_KillCount", "KillCount : {KillCount}"), Args));
}

void UStageResultWindow::UpdateStageResultWindow()
{
	UWorld* CurrentWorld = GetWorld();
	if(CurrentWorld == nullptr) return;

	APlayGameState* PlayGameState = CurrentWorld->GetGameState<APlayGameState>();
	if(PlayGameState == nullptr) return;

	AVSPlayerState* PlayerState = GetOwningPlayerState<AVSPlayerState>();
	if(PlayerState == nullptr) return;

	bool bCleared = PlayGameState->GetStageState() == EStageState::Clear ? true : false;
	SetStageResult(bCleared);

	float FinalStageTime = PlayGameState->GetStageTime();
	SetStageTime(FinalStageTime);

	int FinalMonsterKillCount = PlayGameState->GetMonsterKillCount();
	SetMonsterKillCount(FinalMonsterKillCount);

	float FinalGold = PlayerState->GetGold();
	SetResultGold(FinalGold);
}
