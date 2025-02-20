// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "AbilityTesterUtilityWidget.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class ABILITYTESTEREDITOR_API UAbilityTesterUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TObjectPtr<class UViewport> TesterViewport;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBorder> ViewportBorder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "View", meta = (AllowPrivateAccess = "true"))
	FVector ViewLocation = FVector(-500, -50, 250);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "View", meta = (AllowPrivateAccess = "true"))
	FRotator ViewRotation = FRotator(-90, 0,-90);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TestAbility", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AAbilityTesterActor> TesterActorClass;

	UPROPERTY()
	TObjectPtr<AAbilityTesterActor> TesterActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestAbility", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UGameplayAbility> TestAbility;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TestAbility", meta = (AllowPrivateAccess = "true", ClampMin = "1"))
	int TestLevel = 1;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UDetailsView> UtilityDetails;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UDetailsView> AbilityDetails;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USinglePropertyView> TestLevelDetail;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ApplyBTN;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SaveBTN;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> StateTextBlock;
private:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void InitViewport();

	void SpawnTesterActor();

	UFUNCTION()
	void OnClickedApplyBTN();

	UFUNCTION()
	void OnClickedSaveBTN();

	void OnChangedTestAbility();
	void ApplyTestAbility();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void ReReadAbilityDataCSV();

protected:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

};
