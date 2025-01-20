// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Upgrade/PlayerUpgradeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/AssetManager.h"
#include "ProjectVS.h"
#include "Player/Upgrade/UpgradeDataAsset.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"
#include "Ability/AttributeSet_Default.h"
#include "Ability/AttributeSet_Player.h"
#include "Player/InitAttributeEffect_Player.h"

#if WITH_EDITOR
	#define bDebug true
#endif

UPlayerUpgradeSubsystem* UPlayerUpgradeSubsystem::PUSSinglton = nullptr;

UPlayerUpgradeSubsystem::UPlayerUpgradeSubsystem()
{
}

UPlayerUpgradeSubsystem* UPlayerUpgradeSubsystem::GetPlayerUpgradeSubsystem()
{
	return PUSSinglton;
}

void UPlayerUpgradeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	PUSSinglton = this;

	bLoadComplete = false;

	StartLoadUpgradeDataAsset();
}

void UPlayerUpgradeSubsystem::Deinitialize()
{
	PUSSinglton = nullptr;
}

FPrimaryAssetType UPlayerUpgradeSubsystem::GetTargetPrimaryAssetType() const
{
	return FPrimaryAssetType("UpgradeData");
}

void UPlayerUpgradeSubsystem::GetAssetIdsToLoad(TArray<struct FPrimaryAssetId>& Results) const
{
	FPrimaryAssetType TargetType = GetTargetPrimaryAssetType();

	//일단 전체 추가
	UAssetManager& AssetManager = UAssetManager::Get();

	AssetManager.GetPrimaryAssetIdList(TargetType, Results);
}

void UPlayerUpgradeSubsystem::StartLoadUpgradeDataAsset()
{
	LOG_WARNING(TEXT("Start Load UpgradeDataAsset"));

	UAssetManager& AssetManager = UAssetManager::Get();

	TArray<FPrimaryAssetId> Ids;
	GetAssetIdsToLoad(Ids);

	AssetManager.LoadPrimaryAssets(Ids, TArray<FName>({"TitleMenu"}), FStreamableDelegate::CreateUObject(this, &UPlayerUpgradeSubsystem::CompleteLoadUpgradDataAsset));
}

void UPlayerUpgradeSubsystem::CompleteLoadUpgradDataAsset()
{
	LOG_WARNING(TEXT("Complete Load UpgradeDataAsset"));
	UAssetManager& AssetManager = UAssetManager::Get();

	FPrimaryAssetType TargetType = GetTargetPrimaryAssetType();

	TArray<UObject*> Assets;
	AssetManager.GetPrimaryAssetObjectList(TargetType, Assets);

	for (auto& Asset : Assets)
	{
		UUpgradeDataAsset* DataAsset = Cast<UUpgradeDataAsset>(Asset);

		UpgradeAssets.Add(DataAsset->GetUpgradeName(), DataAsset);
	}

	bLoadComplete = true;
	OnUpgradeDataLoadCompleteDelegate.Broadcast(this);
	OnUpgradeDataLoadCompleteDelegate.Clear();
}

const UUpgradeDataAsset* UPlayerUpgradeSubsystem::FindUpgradeAsset(const FName& InKey) const
{
	if (const UUpgradeDataAsset* const* FindedAsset = UpgradeAssets.Find(InKey))
	{
		return *FindedAsset;
	}

	return nullptr;
}

void UPlayerUpgradeSubsystem::GetAllUpgradeAssets(TArray<const UUpgradeDataAsset*>& Out)
{
	Out.Empty();
	Out.Reserve(UpgradeAssets.Num());

	for (const auto& Asset : UpgradeAssets)
	{
#if WITH_EDITOR
		ensure(IsValid(Asset.Value));
#endif

		Out.Add(Asset.Value);
	}
}


#if WITH_EDITOR
	#undef bDebug
#endif
