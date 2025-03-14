// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectVSGameMode.generated.h"

UCLASS(minimalapi)
class AProjectVSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectVSGameMode(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	virtual void OnPostLogin(AController* NewPlayer) override;

private:


	void StartNewStage();

	void OnMonsterKilled(class AVSMonster* InMonster);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Monster", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	TObjectPtr<class USpawnRule> DefaultSpawnRule;

	FTimerHandle TimerHandle_DefaultSpawnMonster;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UStageResultWindow> StageResultWidgetClass;

private:
	FTimerHandle TimerHandle_UpdateStageTime;
	void UpdateStageTime(float InTime);


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameRule", meta = (AllowPrivateAccess = "true", ClampMin = "1"))
	float ClearSeconds = 10;

	bool bClearGame = false;

	void OnTimeUpClearGame();

public:
	void OnPlayerDead(class AVSPlayerState* InPlayer);
	void OnGameOver();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AVSPaperTileMapActor> TileActorClass;

	FVector TileBoxExtent;

	TMap<int, TMap<int, TWeakObjectPtr<AVSPaperTileMapActor>>> Map_WeakTiles;

	FTimerHandle TimerHandle_CheckTile;

private:

	void SpawnAroundTiles(int X, int Y);

	AVSPaperTileMapActor* SpawnTile(int X, int Y);

	void SetWeakTile(int X, int Y, AVSPaperTileMapActor* InTile);



public:
	void CheckUnlimitedTiles();
};



