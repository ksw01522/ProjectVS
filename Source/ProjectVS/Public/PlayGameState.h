// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PlayGameState.generated.h"


UENUM(BlueprintType)
enum class EStageState : uint8
{
	None = 0 UMETA(Hidden),
	Idle,
	Playing,
	Clear,
	Fail
};

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMonsterKillCountChanged, int, int)

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStageTimeChanged, float, float)


/**
 * 
 */
UCLASS()
class PROJECTVS_API APlayGameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	APlayGameState();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


private:
	UPROPERTY(ReplicatedUsing = OnRep_StageState)
	EStageState StageState;

	UFUNCTION()
	void OnRep_StageState(EStageState OldState);
public:
	void SetStageState(EStageState NewState);
	EStageState GetStageState() const {return StageState;}

private:
	UPROPERTY(ReplicatedUsing = OnRep_MonsterKillCount)
	int MonsterKillCount;

	FOnMonsterKillCountChanged OnMonsterKillCountChanged;

	UFUNCTION()
	void OnRep_MonsterKillCount(int OldMonsterKillCount);

public:
	FOnMonsterKillCountChanged& GetOnMonsterKillCountChangedDelegate() { return OnMonsterKillCountChanged; }

	void SetMonsteKillCount(int NewCount);

	int GetMonsterKillCount() const {return MonsterKillCount; }


private:
	UPROPERTY(ReplicatedUsing = OnRep_StageTime)
	float StageTime;

	FOnStageTimeChanged OnStageTimeChangedDelegate;

	UFUNCTION()
	void OnRep_StageTime(float OldStageTime);

public:
	void UpdateStageTime(float InTime);
	void SetStageTime(float NewTime);
	float GetStageTime() const { return StageTime;}

	FOnStageTimeChanged& GetOnStageTimeChangedDelegate() {return OnStageTimeChangedDelegate;}
};
