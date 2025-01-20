// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayAbilitySpecHandle.h"
#include "AbilityTesterActor.generated.h"

UCLASS()
class AAbilityTesterActor : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAbilityTesterActor();

private:
	UPROPERTY()
	TObjectPtr<class UAbilitySystemComponent> AbilitySystem;

	UPROPERTY()
	TSubclassOf<class UGameplayAbility> TestAbility;

	FGameplayAbilitySpecHandle TestAbilitySpecHandle;

	UPROPERTY()
	TObjectPtr<class UAttributeSet_Default> ATS_Default;

	UPROPERTY()
	TObjectPtr<class UAttributeSet_Player> ATS_Player;

private:
	UPROPERTY(EditAnywhere, Category = "Attribute")
	float MaxHP = 50;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float DamageRate = 1;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float CoolTimeRate = 1;
	
	UPROPERTY(EditAnywhere, Category = "Attribute")
	float ProjectileSpeed = 1;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float ProjectileCount = 0;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float EffectTimeRate = 1;

	UPROPERTY(EditAnywhere, Category = "Attribute")
	float EffectRangeRate = 1;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystem; }

	void SetTestAbility(TSubclassOf<UGameplayAbility> InAbility, int TestLevel);

	bool TryActivateTestAbility() const;
};
