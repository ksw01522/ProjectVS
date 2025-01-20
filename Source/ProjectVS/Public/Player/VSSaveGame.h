// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "VSSaveGame.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGoldChanged, int, int)

/**
 * 
 */
UCLASS()
class PROJECTVS_API UVSSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UVSSaveGame();

private:
	UPROPERTY(SaveGame)
	int Gold;

	mutable FOnGoldChanged OnGoldChangedDelegate;

public:
	int GetGold() const {return Gold;}

	void SetGold(int NewGold);

	void GainGold(int InGold);
	void LoseGold(int InGold);

	FOnGoldChanged& GetOnGoldChangedDelegate() const { return OnGoldChangedDelegate; }

private:
	UPROPERTY(SaveGame)
	TMap<FName, int> UpgradeInfo;

public:
	void UpgradePlayer(const FName& Target, int PayGold, int InLevel);
	int GetUpgradeLevel(const FName& Target) const;

};
