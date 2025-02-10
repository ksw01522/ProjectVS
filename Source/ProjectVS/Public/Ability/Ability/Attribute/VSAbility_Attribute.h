// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/VSAbility.h"
#include "VSAbility_Attribute.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTVS_API UVSAbility_Attribute : public UVSAbility
{
	GENERATED_BODY()
	
public:
	UVSAbility_Attribute(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY(EditDefaultsOnly, Category = "AttributeBonus")
	FGameplayAttribute TargetAttribute;

	UPROPERTY(EditDefaultsOnly, Category = "AttributeBonus")
	TEnumAsByte<EGameplayModOp::Type> ModifierOp = EGameplayModOp::Additive;

	UPROPERTY(EditDefaultsOnly, Category = "AttributeBonus", meta = (Categories = "AttributeBonus"))
	FGameplayTag BonuseValueTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient, EditAnywhere, Category = "AttributeBonus", meta = (AllowPrivateAccess = "true"))
	TArray<float> BonusValue;
#endif

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;


#if WITH_EDITOR	
protected:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const override;

	virtual void NativeLoadDataFromDataManager() override;

#endif

private:
	float GetValue(int InAbilityLevel) const;

	

protected:
	virtual FText GetDescriptionText_Implementation() const override;

	virtual FText GetLeveUpDescriptionText_Implementation(int BeforeLevel, int AfterLevel) const override;

};
