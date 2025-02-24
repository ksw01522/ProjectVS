// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/VSAbility.h"
#include "VSAbility_Flamethrower.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTVS_API UVSAbility_Flamethrower : public UVSAbility
{
	GENERATED_BODY()
	
public:
	UVSAbility_Flamethrower(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual FText GetLeveUpDescriptionText_Implementation(const UAbilityBookComponent* InBook, int BeforeLevel, int AfterLevel) const override;

#if WITH_EDITOR
protected:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const override;

	virtual void NativeLoadDataFromDataManager() override; protected:
	
	//virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;

#endif
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Flame", meta = (AllowPrivateAccess = "true", DisplayThumbnail = "true"))
	TSubclassOf<class AFlamethrowerActor> FlamethrowerActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Flame", meta = (Categories = "Damage"))
	FGameplayTag FlameDamageTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Flame")
	TArray<float> FlameDamage;
#endif

	float GetFlameDamage(int InLevel, UAbilitySystemComponent* InASC) const;


	UPROPERTY(EditDefaultsOnly, Category = "Flame", meta = (Categories = "Scale"))
	FGameplayTag FlameScaleTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Flame")
	TArray<float> FlameScale;
#endif

	float GetFlameScale(int InLevel, UAbilitySystemComponent* InASC) const;

	UPROPERTY(EditDefaultsOnly, Category = "Flame", meta = (Categories = "Duration"))
	FGameplayTag FlameDurationTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "Flame")
	TArray<float> FlameDuration;
#endif

	float GetFlameDuration(int InLevel, UAbilitySystemComponent* InASC) const;

};
