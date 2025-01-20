// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeWidget.generated.h"


DECLARE_DELEGATE_RetVal(int, FOnTryUpgradeDelegate);
/**
 * 
 */
UCLASS(Category = "Upgrade", Abstract)
class PROJECTVS_API UUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUpgradeWidget(const FObjectInitializer& Initializer);

protected:
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> MainBorder;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> NameTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> IconBorder;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> IconBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> LevelTextBlock;

	TWeakObjectPtr<const class UUpgradeDataAsset> WeakTargetData;

	TWeakObjectPtr<class UUpgradeWindowWidget> WeakWindow;

	int MaxLevel = 0;
	int CurrentLevel = 0;

private:
	void SetNameText(const FText& InText);

	void SetIconImage(class UTexture2D* InImage);

	void SetMaxLevel(int NewLevel);
	void SetCurrentLevel(int NewLevel);
	void InitUpgradeLevels(int NewCurrentLevel, int NewMaxLevel);

	void RefreshLevelText();

public:
	void SetUpgradeData(const UUpgradeDataAsset* InAsset);

	FText GetNameText() const;

	UTexture2D* GetIconImage() const;

	void TryUpgrade();

	int GetUpgradeGold() const;

	void SetWeakWindow(class UUpgradeWindowWidget* WeakTarget) { WeakWindow = WeakTarget; }

	int GetMaxLevel() const { return MaxLevel; }
	int GetCurrentLevel() const { return CurrentLevel;}
};
