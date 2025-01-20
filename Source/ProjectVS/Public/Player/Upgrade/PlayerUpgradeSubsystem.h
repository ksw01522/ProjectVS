// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Player/Upgrade/UpgradeDataAsset.h"
#include "PlayerUpgradeSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpgradeDataLoadComplete, class UPlayerUpgradeSubsystem*)
/**
 * 
 */
UCLASS()
class PROJECTVS_API UPlayerUpgradeSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPlayerUpgradeSubsystem();

private:
	static UPlayerUpgradeSubsystem* PUSSinglton;

public:
	static UPlayerUpgradeSubsystem* GetPlayerUpgradeSubsystem();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;


private:
	FPrimaryAssetType GetTargetPrimaryAssetType() const;

	void GetAssetIdsToLoad(TArray<struct FPrimaryAssetId>& Results) const;

	void StartLoadUpgradeDataAsset();
	
	void CompleteLoadUpgradDataAsset();
	
private:
	bool bLoadComplete = false;

	FOnUpgradeDataLoadComplete OnUpgradeDataLoadCompleteDelegate;

public:
	bool IsLoadedUpgradeData() const { return bLoadComplete; }

	FOnUpgradeDataLoadComplete& GetOnUpgradeDataLoadComplete() { return OnUpgradeDataLoadCompleteDelegate; }

private:
	UPROPERTY()
	TMap<FName, const class UUpgradeDataAsset*> UpgradeAssets;

public:
	const UUpgradeDataAsset* FindUpgradeAsset(const FName& InKey) const;

	void GetAllUpgradeAssets(TArray<const UUpgradeDataAsset*>& Out);
};
