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
#include "UserWidget.h"
#include "Player/WidgetManagerComponent.h"
#include "Player/VSPlayerState.h"
#include "Tile/VSPaperTileMapActor.h"
#include "Components/BoxComponent.h"

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

void AProjectVSGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	UWorld* World = GetWorld();
	if(World == nullptr) return;

	const AVSPaperTileMapActor* TileCDO = TileActorClass.GetDefaultObject();
	check(TileCDO != nullptr)

	const UBoxComponent* CDOTileBox = TileCDO->GetTileBox();

	TileBoxExtent = CDOTileBox->GetScaledBoxExtent();
	TileBoxExtent.X *= 2;
	TileBoxExtent.Y *= 2;

	AVSPaperTileMapActor* StartTile = SpawnTile(0,0);	
}

void AProjectVSGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

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

	GetWorld()->GetTimerManager().SetTimer(TimerHandle_CheckTile, this, &AProjectVSGameMode::CheckUnlimitedTiles, 5, true, 0.0001);
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

	//임시 OnGameOver
	OnGameOver();
}

void AProjectVSGameMode::OnPlayerDead(AVSPlayerState* InPlayer)
{
	LOG_WARNING(TEXT("On Player Dead"));

	//임시
	OnGameOver();
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

	
	if (AProjectVSPlayerController* FirstLocal = Cast<AProjectVSPlayerController>(GetGameInstance()->GetFirstLocalPlayerController(GetWorld())))
	{
		UWidgetManagerComponent* PlayerWidgetManager = FirstLocal->GetWidgetManager();

		SetPause(FirstLocal);

		UUserWidget* NewWidget = PlayerWidgetManager->AddWidget("StageResultWindow", StageResultWidgetClass);
		if (NewWidget)
		{
			StaticCast<UStageResultWindow*>(NewWidget)->UpdateStageResultWindow();
			NewWidget->AddToViewport(1);
		}
	}
}

void AProjectVSGameMode::SpawnAroundTiles(int X, int Y)
{
	for (int YDist = -1; YDist <= 1; YDist++)
	{
		for(int XDist = -1; XDist <= 1; XDist++)
		{ 
			SpawnTile(X + XDist, Y + YDist);
		}
	}
}

AVSPaperTileMapActor* AProjectVSGameMode::SpawnTile(int X, int Y)
{
	if(Map_WeakTiles.Contains(Y) && Map_WeakTiles[Y].Contains(X)) return nullptr;

	FVector SpawnLocation(X * TileBoxExtent.X, Y * TileBoxExtent.Y, -1 * TileBoxExtent.Z);

	AVSPaperTileMapActor* SpawnedTile = GetWorld()->SpawnActor<AVSPaperTileMapActor>(TileActorClass, SpawnLocation, FRotator::ZeroRotator);
	
	SetWeakTile(X, Y, SpawnedTile);

	return SpawnedTile;
}

void AProjectVSGameMode::SetWeakTile(int X, int Y, AVSPaperTileMapActor* InTile)
{
	Map_WeakTiles.FindOrAdd(Y).FindOrAdd(X) = InTile;
}

void AProjectVSGameMode::CheckUnlimitedTiles()
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PlayerActor = Iterator->Get();
		APawn* PlayerPawn = PlayerActor->GetPawn();

		if(PlayerPawn == nullptr) continue;

		FVector PlayerLocation = PlayerPawn->GetActorLocation();

		int TileX = PlayerLocation.X / TileBoxExtent.X;
		int TileY = PlayerLocation.Y / TileBoxExtent.Y;

		SpawnAroundTiles(TileX, TileY);
	}

}

