// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/VSAbility.h"
#include "VSAbility_Topspin.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVS_API UVSAbility_Topspin : public UVSAbility
{
	GENERATED_BODY()
	
public:
	UVSAbility_Topspin(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Topspin")
	TSubclassOf<class ATopspinActor> TopspinActorClass;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Topspin", meta = (Categories = "Damage"))
	FGameplayTag TopspinDamageTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Topspin")
	TArray<float> TopspinDamage;
#endif

	UPROPERTY(EditDefaultsOnly, Category = "Topspin", meta = (Categories = "Scale"))
	FGameplayTag TopspinScaleTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Topspin")
	TArray<float> TopspinScale;
#endif

	UPROPERTY(EditDefaultsOnly, Category = "Topspin", meta = (Categories = "Count"))
	FGameplayTag TopspinCountTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Topspin")
	TArray<float> TopspinCount;
#endif

public:
	float GetTopspinDamage(int InLevel, UAbilitySystemComponent* InASC) const;

	float GetTopspinScale(int InLevel, UAbilitySystemComponent* InASC) const;

	int GetTopspinCount(int InLevel, UAbilitySystemComponent* InASC) const;

#if WITH_EDITOR
protected:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const override;

	virtual void NativeLoadDataFromDataManager() override;
#endif


};
