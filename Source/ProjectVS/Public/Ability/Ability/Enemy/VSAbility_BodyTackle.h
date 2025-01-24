// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/VSAbility.h"
#include "VSAbility_BodyTackle.generated.h"

/**
 * 
 */
UCLASS(NotBlueprintable)
class PROJECTVS_API UVSAbility_BodyTackle : public UVSAbility
{
	GENERATED_BODY()
	
	UVSAbility_BodyTackle(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	virtual bool CanSetOrAddInBook(const class UAbilityBookComponent& InBook, int NewLevel) const override;

};
