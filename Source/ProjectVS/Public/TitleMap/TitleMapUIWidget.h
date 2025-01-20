// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleMapUIWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTVS_API UTitleMapUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> OpenUpgradeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUpgradeWindowWidget> UpgradeWindow;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> GoldTextBlock;

	FDelegateHandle OnGoldChangedHandle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> QuitButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> StartPlayButton;


	void OnGoldChanged(int PrevGold, int NewGold);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UUpgradeWindowWidget* GetUpgradeWindow() const { return UpgradeWindow; }

private:
	UFUNCTION()
	void StartPlayGame();

	UFUNCTION()
	void OpenUpgradeWindow();

	UFUNCTION()
	void QuitGame();
};
