// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effect/DamageEffectExecutionCalculation.h"
#include "Ability/AttributeSet_Default.h"
#include "AbilitySystemComponent.h"
#include "ProjectVS.h"

struct FDamageExecutionStatics
{
};

static const FDamageExecutionStatics& DamageStatics()
{
	static FDamageExecutionStatics DamageStatics;
	return DamageStatics;
}

UDamageEffectExecutionCalculation::UDamageEffectExecutionCalculation()
{
#if WITH_EDITORONLY_DATA
	ValidTransientAggregatorIdentifiers.AddTag(UAttributeSet_Default::GetDamageTag());
#endif
}

void UDamageEffectExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;


	float Damage = 0;
	ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(UAttributeSet_Default::GetDamageTag(), EvaluationParameters, Damage);

	if (0 < Damage)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UAttributeSet_Default::GetDamageAttribute(), EGameplayModOp::Additive, Damage));
	}
}
