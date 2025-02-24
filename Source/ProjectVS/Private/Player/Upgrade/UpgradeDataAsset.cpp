// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Upgrade/UpgradeDataAsset.h"
#include "ProjectVS.h"

FPrimaryAssetId UUpgradeDataAsset::GetPrimaryAssetId() const
{

	return FPrimaryAssetId("UpgradeData", UpgradeName);
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
