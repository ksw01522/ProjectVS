// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/VSAbility.h"
#include "GameplayEffectTypes.h"
#include "VSAbility_Gundance.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTVS_API UVSAbility_Gundance : public UVSAbility
{
	GENERATED_BODY()
	
public:
	UVSAbility_Gundance(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void PreAddInBook_Implementation(class UAbilityBookComponent* InBook) const override;
	virtual bool CanSetOrAddInBook_Implementation(const class UAbilityBookComponent* InBook, int NewLevel) const override;

	virtual FText GetLeveUpDescriptionText_Implementation(const UAbilityBookComponent* InBook, int BeforeLevel, int AfterLevel) const override;


#if WITH_EDITOR
protected:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const override;

	virtual void NativeLoadDataFromDataManager();
#endif

private:
	UPROPERTY(EditDefaultsOnly, Category = "Gundance", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AGundanceActor> BladeEffecterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Gundance", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AGundanceActor> ShotEffecterClass;

	UPROPERTY(EditDefaultsOnly, Category = "Gundance", meta = (AllowPrivateAccess = "true", Categories = "Damage"))
	FGameplayTag BladeDamageTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient, EditAnywhere, Category = "Gundance", meta = (AllowPrivateAccess = "true"))
	TArray<float> BladeDamage;
#endif

	FGameplayEffectSpecHandle CachedBladeDamageEffectSpecHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Gundance", meta = (AllowPrivateAccess = "true", Categories = "Damage"))
	FGameplayTag ShotDamageTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient, EditAnywhere, Category = "Gundance", meta = (AllowPrivateAccess = "true"))
	TArray<float> ShotDamage;
#endif

	FGameplayEffectSpecHandle CachedShotDamageEffectSpecHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Gundance", meta = (AllowPrivateAccess = "true", Categories = "Scale"))
	FGameplayTag DanceScaleTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient, EditAnywhere, Category = "Gundance", meta = (AllowPrivateAccess = "true"))
	TArray<float> DanceScale;
#endif

	FVector CachedDanceScale;

	UPROPERTY(EditDefaultsOnly, Category = "Gundance", meta = (AllowPrivateAccess = "true", Categories = "Count"))
	FGameplayTag DanceCountTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient, EditAnywhere, Category = "Gundance", meta = (AllowPrivateAccess = "true"))
	TArray<float> DanceCount;
#endif

	UPROPERTY(EditDefaultsOnly, Category = "Gundance", meta = (AllowPrivateAccess = "true", Categories = "Time"))
	FGameplayTag TimeBetweenDanceTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient, EditAnywhere, Category = "Gundance", meta = (AllowPrivateAccess = "true"))
	TArray<float> TimeBetweenDance;
#endif

	UPROPERTY(EditAnywhere, Category = "Gundance", meta = (AllowPrivateAccess = "true"))
	TArray<float> BladeDegrees;

	UPROPERTY(EditAnywhere, Category = "Gundance", meta = (AllowPrivateAccess = "true"))
	TArray<float> ShotDegrees;

public:
	float GetBladeDamage(int InLevel, UAbilitySystemComponent* InASC) const;
	float GetShotDamage(int InLevel, UAbilitySystemComponent* InASC) const;
	float GetDanceScale(int InLevel, UAbilitySystemComponent* InASC) const;
	float GetDanceCount(int InLevel, UAbilitySystemComponent* InASC) const;
	float GetTimeBetweenDance(int InLevel, UAbilitySystemComponent* InASC) const;

private:
	UFUNCTION()
	void ShotAndBlade(int32 ActionNumber);

	UFUNCTION()
	void OnFinishedGundance(int32 ActionNumber);
};
