// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MonsterWorldManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMonsterDeadDelegate, class AVSMonster*)

/**
 * 
 */
UCLASS()
class PROJECTVS_API UMonsterWorldManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	TSet<TWeakObjectPtr<class AVSMonster>> WeakMonsters;

	FOnMonsterDeadDelegate OnMonsterDeadDel;

private:
	void OnMonsterDead(class AVSMonster* Target);

protected:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;


	virtual void OnWorldBeginPlay(UWorld& InWorld) override;


public:
	TArray<class AVSMonster*> SpawnMonseters(const FName& InCode, float Level, const TArray<FVector>& SpawnLocations);

	int GetMonsterNum() const { return WeakMonsters.Num(); }

	void GetMonsters(TArray<AVSMonster*>& Out) const;

	FOnMonsterDeadDelegate& GetOnMonsterDeadDelegate() {return OnMonsterDeadDel;}

	void RemoveMonster(AVSMonster* Target);
};
