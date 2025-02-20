// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerMainUIWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTVS_API UPlayerMainUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerMainUIWidget(const FObjectInitializer& Initializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void RegisterAbilityBook(class UAbilityBookComponent* NewBook);
	
	void UnregisterAbilityBook();

protected:
	virtual void OnRegisterAbilityBook(UAbilityBookComponent* NewBook);
	virtual void OnUnregisterAbilityBook(UAbilityBookComponent* PrevBook);

private:
	UFUNCTION()
	void OnUpdateAbilityBook(UAbilityBookComponent* InBook);

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<class UAbilityBookComponent> WeakAbilityBook;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> KillCountText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> StageTimeText;

	int StageMinute = 0;
	int StageSeconds = 0;

	FDelegateHandle KillCountHandle;
	FDelegateHandle StageTimeHandle;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> LevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class URichTextBlock> GoldText;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> EXPBar;

	float CurrentEXP = 0;
	float EXPForLevelUp = 0;

private:
	void SetCurrentEXP(float NewEXP);
	void SetEXPForLevelUp(float NewEXPForLevelUp);

	void OnCurrentEXPChanged(const struct FOnAttributeChangeData& ChangedData);
	void OnEXPForLevelUpChanged(const FOnAttributeChangeData& ChangedData);

	void UpdateEXPBar();

	void OnLevelChanged(const FOnAttributeChangeData& ChangedData);

	void OnGoldChanged(const FOnAttributeChangeData& ChangedData);

	void SetGold(int NewGold);

public:
	void SetKillCount(int OldCount, int NewCount);

	void SetStageTime(float OldTime, float NewTime);

	void SetLevel(int NewLevel);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityIcon", meta = (AllowPrivateAccess = "true"))
	float Padding_Horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityIcon", meta = (AllowPrivateAccess = "true"))
	float Padding_Vertical;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> ActiveIconBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UHorizontalBox> PassiveIconBox;

protected:
	void UpdateIconBox(UHorizontalBox* IconBox, TArray<const struct FAbilityPage*>& Pages);

public:
	void UpdateIconBoxs();
};
