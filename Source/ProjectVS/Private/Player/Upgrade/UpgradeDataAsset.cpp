// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Upgrade/UpgradeDataAsset.h"
#include "ProjectVS.h"

FPrimaryAssetId UUpgradeDataAsset::GetPrimaryAssetId() const
{
	if(!HasAnyFlags(RF_ClassDefaultObject)) return FPrimaryAssetId();

	LOG_ERROR(TEXT("Asset Name : %s"), *GetFName().ToString());

	return FPrimaryAssetId("UpgradeData", GetFName());
}

float UUpgradeDataAsset::GetPower(int Level) const
{
	ensure(0 <= Level && Level <= MaxLevel);

	return PowerPerLevel * Level;
}

int UUpgradeDataAsset::GetGoldForUpgrade(int Level) const
{
	ensure(0 <= Level && Level <= MaxLevel);

	return BaseGold + Level * GoldPerLevel;
}
