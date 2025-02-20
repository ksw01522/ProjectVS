// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/VSAbility.h"
#include "VSAbility_CannonBall.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS(Abstract)
class PROJECTVS_API UVSAbility_CannonBall : public UVSAbility
{
	GENERATED_BODY()

public:
	UVSAbility_CannonBall(const FObjectInitializer& ObjectInitailizer);

protected:
	virtual void ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "CannonBall")
	TSubclassOf<class ACannonBallActor> CannonBallActorClass;

private:
	UPROPERTY(EditDefaultsOnly, Category = "CannonBall", meta = (Categories = "Damage"))
	FGameplayTag CannonBallDamageTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "CannonBall")
	TArray<float> CannonBallDamage;
#endif

	UPROPERTY(EditDefaultsOnly, Category = "CannonBall", meta = (Categories = "Damage"))
	FGameplayTag ExplosionDamageTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "CannonBall")
	TArray<float> ExplosionDamage;
#endif


	UPROPERTY(EditDefaultsOnly, Category = "CannonBall", meta = (Categories = "Scale"))
	FGameplayTag CannonBallScaleTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "CannonBall")
	TArray<float> CannonBallScale;
#endif

	UPROPERTY(EditDefaultsOnly, Category = "CannonBall", meta = (Categories = "Speed"))
	FGameplayTag CannonBallSpeedTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = "CannonBall")
	TArray<float> CannonBallSpeed;
#endif

protected:
	FQuat CreateCannonBallRoation(AActor* AvatarActor) const;

public:
	float GetCannonBallDamage(int InLevel, UAbilitySystemComponent* InASC) const;

	float GetExplosionDamage(int InLevel, UAbilitySystemComponent* InASC) const;

	float GetCannonBallScale(int InLevel, UAbilitySystemComponent* InASC) const;

	float GetCannonBallSpeed(int InLevel, UAbilitySystemComponent* InASC) const;


#if WITH_EDITOR
protected:
	//virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const override;

	virtual void NativeLoadDataFromDataManager() override;
#endif

};
