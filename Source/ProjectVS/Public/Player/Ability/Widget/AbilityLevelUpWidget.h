// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityLevelUpWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVS_API UAbilityLevelUpWidget : public UUserWidget
{
	GENERATED_BODY()

public:	
	UAbilityLevelUpWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;



private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> IconImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> NameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> DescriptionText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> LevelText;

	FName AbilityCode;
	float CurrentLevel = 0;
	float TargetLevel = 0;

private:
	void UpdateAbilityLevelUpWidget();

	void SetIcon(class UTexture2D* InTexture);
	void SetNameText(const FText& InText);
	void SetDescriptionText(const FText& InText);
	void SetLevelText(float InLevel);

public:
	FName GetAbilityCode() const { return AbilityCode; }
	float GetTargetLevel() const { return TargetLevel; }

	void SetAbilityLevelUpInfo(const struct FAbilityLevelUpTargetInfo& InInfo);

	void OnSelectedLevelUp(class AProjectVSPlayerController* PlayerController);

	TMulticastDelegate<void ()> OnSelectedLevelUpDelegate;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

};
