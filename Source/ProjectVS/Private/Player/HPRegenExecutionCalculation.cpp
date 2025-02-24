// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HPRegenExecutionCalculation.h"
#include "Player/Ability/AttributeSet_Player.h"
#include "Ability/AttributeSet_Default.h"

struct FHPRegenStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(HPRegen);

	FHPRegenStatics()
	{
		HPRegenDef = FGameplayEffectAttributeCaptureDefinition(UAttributeSet_Player::GetHPRegenAttribute(), EGameplayEffectAttributeCaptureSource::Source, false);
	}

};

static const FHPRegenStatics& HPRegenStatics()
{
	static FHPRegenStatics HRStatics;
	return HRStatics;
}


UHPRegenExecutionCalculation::UHPRegenExecutionCalculation()
{
	RelevantAttributesToCapture.Add(FHPRegenStatics().HPRegenDef);
}

void UHPRegenExecutionCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters Aggregator;
	Aggregator.SourceTags = TargetTags;

	float RegenValue = 0;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HPRegenStatics().HPRegenDef, Aggregator, RegenValue);

	if (0 < RegenValue)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UAttributeSet_Default::GetHPAttribute(), EGameplayModOp::Additive, RegenValue));
	}

}
