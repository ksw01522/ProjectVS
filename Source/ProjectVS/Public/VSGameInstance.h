// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VSGameInstance.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnGoldChanged, int, int)

/**
 * 
 */
UCLASS()
class PROJECTVS_API UVSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UVSGameInstance();

protected:
	virtual void Init() override;

public:
	void SaveVSGame();

private:
	void LoadVSGame();

private:
	UPROPERTY(Transient)
	int Gold;

	UPROPERTY(Transient)
	TObjectPtr<class UVSSaveGame> CurrentSave;

	UPROPERTY(EditDefaultsOnly, Category = "Level")
	TSoftObjectPtr<class UWorld> TitleLevel;

	UPROPERTY(EditDefaultsOnly, Category = "Level")
	TSoftObjectPtr<class UWorld> PlayLevel;

public:
	int GetGold() const;

	void GainGold(int InGold);
	void LoseGold(int InGold);

	FOnGoldChanged& GetOnGoldChangedDelegate() const;

	bool CanPayGold(int TargetGold);

	void UpgradePlayer(const FName& Target, int PayGold, int InLevel);
	int GetUpgradeLevel(const FName& Target) const;

	void LoadPlayMap();
	void LoadTitleMap();
};
