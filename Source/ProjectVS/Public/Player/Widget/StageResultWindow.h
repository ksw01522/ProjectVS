// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageResultWindow.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTVS_API UStageResultWindow : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> StageResultText;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetText")
	FText ClearText;

	UPROPERTY(EditDefaultsOnly, Category = "WidgetText")
	FText FailText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> ResultGoldText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> KillCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> StageTimeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ToTitleBTN;


protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void OnClickedToTileBTN();

public:
	void SetStageResult(bool bCleared);

	void SetResultGold(float NewGold);

	void SetStageTime(float NewTime);

	void SetMonsterKillCount(int NewCount);

	void UpdateStageResultWindow();
};
