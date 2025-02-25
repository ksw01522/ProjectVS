// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UpgradeDataAsset.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTVS_API UUpgradeDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FText UpgradeNameText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName UpgradeName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int MaxLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float PowerPerLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int BaseGold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int GoldPerLevel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true", AssetBundles = "TitleMenu"))
	TSoftObjectPtr<class UTexture2D> Icon;

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	FText GetUpgradeNameText() const { return UpgradeNameText; }

	float GetPower(int Level) const;

	FName GetUpgradeName() const { return UpgradeName; }

	int GetMaxLevel() const { return MaxLevel; }
	
	int GetGoldForUpgrade(int Level) const;

	UTexture2D* GetIconImage() const { return Icon.Get(); }

	FText GetDescriptionText() const;
};
