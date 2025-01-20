// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "DamageEffect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVS_API UDamageEffect : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UDamageEffect();

	static FGameplayEffectSpecHandle MakeDamageEffectSpec(float NewDamage, const FGameplayEffectContextHandle& ContextHandle, TSubclassOf<UDamageEffect> DamageClass = UDamageEffect::StaticClass());
};
