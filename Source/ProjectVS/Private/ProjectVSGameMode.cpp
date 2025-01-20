// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectVSGameMode.h"
#include "ProjectVSPlayerController.h"
#include "ProjectVSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Player/VSPlayerState.h"
#include "PlayGameState.h"
#include "ProjectVS.h"
#include "SpawnRule/SpawnRule.h"
#include "Character/Monster/MonsterWorldManager.h"
#include "Character/Monster/VSMonster.h"
#include "ProjectVSPlayerController.h"
#include "Player/Widget/StageResultWindow.h"
#include "Player/WidgetManagerComponent.h"

AProjectVSGameMode::AProjectVSGameMode(const FObjectInitializer& ObjectInitializer) :
			Super(ObjectInitializer)
{
	// use our custom PlayerController class
	PlayerControllerClass = AProjectVSPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/Blueprints/BP_ProjectVSCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Player/Blueprints/BP_ProjectVSController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	PlayerStateClass = AVSPlayerState::StaticClass();

	HUDClass = nullptr;

	GameStateClass = APlayGameState::StaticClass();
}

void AProjectVSGameMode::OnPostLogin(AController* NewPlayer)
{
	ENetRole ControllerNetRole = NewPlayer->GetRemoteRole();
	ENetMode NetMode = GetNetMode();

	if (NetMode == ENetMode::NM_DedicatedServer && ControllerNetRole == ENetRole::ROLE_SimulatedProxy)
	{
		return;
	}
	

	APlayGameState* PlayGameState = GetGameState<APlayGameState>();
	if (PlayGameState->GetStageState() == EStageState::Idle)
	{
		StartNewStage();
	}
}

void AProjectVSGameMode::StartNewStage()
{
	APlayGameState* PlayGameState = GetGameState<APlayGameState>();

	PlayGameState->SetStageState(EStageState::Playing);

	PlayGameState->SetStageTime(0);
	
	const float UpdateTime = 1;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_UpdateStageTime,
											FTimerDelegate::CreateUObject(this, &AProjectVSGameMode::UpdateStageTime, UpdateTime),
											UpdateTime, true);

	ENetMode CurrentNetMode = GetNetMode();

	if (DefaultSpawnRule && CurrentNetMode != ENetMode::NM_Client)
	{
		TWeakObjectPtr<USpawnRule> WeakDefaultSpawRule = DefaultSpawnRule;
		TWeakObjectPtr<UWorld> WeakWorld = GetWorld();
		const float DefaultSpawnInterval = 2.5f;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_DefaultSpawnMonster,
												FTimerDelegate::CreateLambda([WeakDefaultSpawRule, WeakWorld](){
													if (WeakDefaultSpawRule.IsValid() && WeakWorld.IsValid())
													{
														WeakDefaultSpawRule->SpawnMonster(*WeakWorld);
													}
												}),
												 DefaultSpawnInterval, true);
	}
	if (UMonsterWorldManager* MonsterWM = GetWorld()->GetSubsystem<UMonsterWorldManager>())
	{
		MonsterWM->GetOnMonsterDeadDelegate().AddUObject(this, &AProjectVSGameMode::OnMonsterKilled);
	}
}



void AProjectVSGameMode::UpdateStageTime(float InTime)
{
	APlayGameState* PlayGameState = GetGameState<APlayGameState>();

	PlayGameState->UpdateStageTime(InTime);

	if (!bClearGame)
	{
		float CurrentTime = PlayGameState->GetStageTime();

		if(ClearSeconds <= CurrentTime) { OnTimeUpClearGame(); }
	}
}

void AProjectVSGameMode::OnMonsterKilled(AVSMonster* InMonster)
{
	float BountyEXP = InMonster->GetBountyEXP();
	float BountyGold = InMonster->GetBountyGold();

	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AVSPlayerState* PlayerState = It->Get()->GetPlayerState<AVSPlayerState>();

		PlayerState->GainEXP(BountyEXP);
		PlayerState->GainGold(BountyGold);
	}
}

void AProjectVSGameMode::OnTimeUpClearGame()
{
	LOG_WARNING(TEXT("On TimeUp ClearGame"))

	bClearGame = true;
}

void AProjectVSGameMode::OnPlayerDead()
{
	LOG_WARNING(TEXT("On Player Dead"));

	

}

void AProjectVSGameMode::OnGameOver()
{
	APlayGameState* PlayGameState = GetGameState<APlayGameState>();

	if (bClearGame)
	{
		PlayGameState->SetStageState(EStageState::Clear);
	}
	else
	{
		PlayGameState->SetStageState(EStageState::Fail);
	}

	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		AVSPlayerState* PlayerState = It->Get()->GetPlayerState<AVSPlayerState>();

		AProjectVSPlayerController* PlayerController = Cast<AProjectVSPlayerController>(It->Get());

		PlayerState->SaveGold();
	}
}

