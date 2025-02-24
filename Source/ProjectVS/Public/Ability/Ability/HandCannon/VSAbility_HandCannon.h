// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/VSAbility.h"
#include "VSAbility_HandCannon.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTVS_API UVSAbility_HandCannon : public UVSAbility
{
	GENERATED_BODY()
	
public:
	UVSAbility_HandCannon(const FObjectInitializer& ObjectInitializer);


protected:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual FText GetLeveUpDescriptionText_Implementation(const UAbilityBookComponent* InBook, int BeforeLevel, int AfterLevel) const override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TSubclassOf<class AHandCannonFireActor> FireActorClass;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Fire", meta = (Categories = "Damage"))
	FGameplayTag FireDamageTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TArray<float> FireDamage;
#endif

	UPROPERTY(EditDefaultsOnly, Category = "Fire", meta = (Categories = "Scale"))
	FGameplayTag FireScaleTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TArray<float> FireScale;
#endif

	UPROPERTY(EditDefaultsOnly, Category = "Fire", meta = (Categories = "Count"))
	FGameplayTag FireCountTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TArray<float> FireCount;
#endif

protected:
	FQuat CreateFireRoation(AActor* AvatarActor) const;

public:
	float GetFireDamage(int InLevel, UAbilitySystemComponent* InASC) const;

	float GetFireScale(int InLevel, UAbilitySystemComponent* InASC) const;

	int GetFireCount(int InLevel, UAbilitySystemComponent* InASC) const;

#if WITH_EDITOR
protected:
	//virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const override;

	virtual void NativeLoadDataFromDataManager() override;
#endif

private:
	void FireHandCannon(const FGameplayAbilitySpecHandle Handle, TWeakObjectPtr<class UAbilitySystemComponent> InASC, FQuat Rotation, float Damage, float Scale, int Count);
};
