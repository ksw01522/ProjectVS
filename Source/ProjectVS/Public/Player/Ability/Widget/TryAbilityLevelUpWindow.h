// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TryAbilityLevelUpWindow.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVS_API UTryAbilityLevelUpWindow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UTryAbilityLevelUpWindow(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UVerticalBox> LevelUpWidgetBox;

	UPROPERTY()
	TArray<TObjectPtr<class UAbilityLevelUpWidget>> LevelUpWidgets;

protected:
	virtual void NativeConstruct() override;

private:
	void OnSlectedLevelUpWidget();

public:
	void AddLevelUpWidget(class UAbilityLevelUpWidget* Target);
	void ClearLevelUpWidgetBox();

	void HiddenLevelUpWidgets();
	void SetAbilityLevelUp(const TArray<struct FAbilityLevelUpTargetInfo>& Targets);

};
