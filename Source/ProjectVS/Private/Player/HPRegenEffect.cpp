// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HPRegenEffect.h"
#include "Player/HPRegenExecutionCalculation.h"
#include "Player/Ability/AttributeSet_Player.h"

UHPRegenEffect::UHPRegenEffect()
{
	Period = 1;

	DurationPolicy = EGameplayEffectDurationType::Infinite;

	FGameplayEffectExecutionDefinition HPRegenExecutionDef;
	HPRegenExecutionDef.CalculationClass = UHPRegenExecutionCalculation::StaticClass();

	FGameplayEffectExecutionScopedModifierInfo HPRegenCaptureInfo(FGameplayEffectAttributeCaptureDefinition(UAttributeSet_Player::GetHPRegenAttribute(), EGameplayEffectAttributeCaptureSource::Source, false));
	HPRegenCaptureInfo.TargetTags.IgnoreTags = FGameplayTagContainer(FGameplayTag::RequestGameplayTag("State.Dead"));

	HPRegenExecutionDef.CalculationModifiers.Add(HPRegenCaptureInfo);

	Executions.Add(HPRegenExecutionDef);


}
