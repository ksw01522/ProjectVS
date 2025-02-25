// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleMap/UpgradeWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"

#include "Player/Upgrade/UpgradeDataAsset.h"
#include "VSGameInstance.h"

#include "ProjectVS.h"

#include "TitleMap/UpgradeWindowWidget.h"
#include "Components/RichTextBlock.h"

UUpgradeWidget::UUpgradeWidget(const FObjectInitializer& Initializer) : Super(Initializer)
{
	SetIsFocusable(true);
	SetVisibility(ESlateVisibility::Visible);

	WeakTargetData.Reset();
}

FReply UUpgradeWidget::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	LOG_WARNING(TEXT("On Focused Upgarde : %s"), *GetNameText().ToString());

	if (WeakWindow.IsValid())
	{
		WeakWindow->OnFocusedUpgradeWidget(this);
	}

	return FReply::Handled();
}

void UUpgradeWidget::SetNameText(const FText& InText)
{
	NameTextBlock->SetText(InText);
}

void UUpgradeWidget::SetIconImage(UTexture2D* InImage)
{
	IconBlock->SetBrushFromTexture(InImage);
}

void UUpgradeWidget::SetCurrentLevel(int NewLevel)
{
	if(CurrentLevel == NewLevel) return;

	CurrentLevel = NewLevel;

	RefreshLevelText();
}

void UUpgradeWidget::InitUpgradeLevels(int NewCurrentLevel, int NewMaxLevel)
{
	CurrentLevel = NewCurrentLevel;
	MaxLevel = NewMaxLevel;

	RefreshLevelText();
}

void UUpgradeWidget::RefreshLevelText()
{
	LevelTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentLevel, MaxLevel)));
}

void UUpgradeWidget::SetMaxLevel(int NewLevel)
{
	if(MaxLevel == NewLevel) return;

	MaxLevel = NewLevel;

	RefreshLevelText();
}

void UUpgradeWidget::SetUpgradeData(const UUpgradeDataAsset* InAsset)
{
	if(InAsset == WeakTargetData.Get()) return;

	WeakTargetData = InAsset;

	if (WeakTargetData.IsValid())
	{
		SetNameText(WeakTargetData->GetUpgradeNameText());

		SetIconImage(WeakTargetData->GetIconImage());

		int MLevel = WeakTargetData->GetMaxLevel();

		UVSGameInstance* VSGameInst = GetGameInstance<UVSGameInstance>();
		int CLevel = VSGameInst->GetUpgradeLevel(WeakTargetData->GetUpgradeName());

		InitUpgradeLevels(CLevel, MLevel);
	}
	else
	{
		SetNameText(FText::GetEmpty());

		SetIconImage(nullptr);

		InitUpgradeLevels(0,0);
	}
}

FText UUpgradeWidget::GetNameText() const
{
	if (WeakTargetData.IsValid())
	{
		return WeakTargetData->GetUpgradeNameText();
	}
	else
	{
		return FText::GetEmpty();
	}
}

FText UUpgradeWidget::GetDescriptionText() const
{
	if (WeakTargetData.IsValid())
	{
		return WeakTargetData->GetDescriptionText();
	}
	else
	{
		return FText::GetEmpty();
	}
}

UTexture2D* UUpgradeWidget::GetIconImage() const
{
	if (WeakTargetData.IsValid())
	{
		return WeakTargetData->GetIconImage();
	}
	else
	{
		return nullptr;
	}

	
}

void UUpgradeWidget::TryUpgrade()
{
	if(!WeakTargetData.IsValid()) return;

	int GoldForUpgrade = WeakTargetData->GetGoldForUpgrade(CurrentLevel);

	UVSGameInstance* VSGameInst = GetGameInstance<UVSGameInstance>();
	if(!VSGameInst->CanPayGold(GoldForUpgrade)) return;

	FName TargetName = WeakTargetData->GetUpgradeName();
	
	VSGameInst->UpgradePlayer(TargetName, GoldForUpgrade, CurrentLevel + 1);
	int NewLevel = VSGameInst->GetUpgradeLevel(TargetName);

	SetCurrentLevel(NewLevel);
}


int UUpgradeWidget::GetUpgradeGold() const
{
	if (WeakTargetData.IsValid())
	{
		return WeakTargetData->GetGoldForUpgrade(CurrentLevel);
	}

	return 0;
}


