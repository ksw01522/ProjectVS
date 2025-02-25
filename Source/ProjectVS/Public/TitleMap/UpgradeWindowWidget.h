// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UpgradeWindowWidget.generated.h"

/**
 * 
 */
UCLASS(Category = "Upgrade", Abstract)
class PROJECTVS_API UUpgradeWindowWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UUpgradeWindowWidget(const FObjectInitializer& Initializer);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> WindowBorder;

	int BaseGold;

	int GoldPerLevel;

	TWeakObjectPtr<class UUpgradeWidget> FocusedUpgrade;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> FocusedName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> FocusedIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> UpgradeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> UpgradeGoldText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> DescriptionText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ExitButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> UpgradeWidgetPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UpgradeWidget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUpgradeWidget> UpgradeWidgetClass;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = WidgetStyle, meta = (AllowPrivateAccess = "true", ClampMin = "1"))
	int MaxColumn = 1; 

private:
	UFUNCTION()
	void InitializeUpgradeWidgets(class UPlayerUpgradeSubsystem* InSystem);

	UFUNCTION()
	void OnUpgradeButtonClick();

	void RefreshFocusedWidget();

	UFUNCTION()
	void ExitWindow();
protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

public:
	void OnFocusedUpgradeWidget(UUpgradeWidget* Target);

};
