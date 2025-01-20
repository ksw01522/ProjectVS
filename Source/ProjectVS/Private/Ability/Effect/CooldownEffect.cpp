// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effect/CooldownEffect.h"

FName UCooldownEffect::CoolTimeSetByCallerName = FName("CoolTime.Duration");

UCooldownEffect::UCooldownEffect()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;

	FSetByCallerFloat DurationValue;
	DurationValue.DataName = CoolTimeSetByCallerName;

	DurationMagnitude = DurationValue;
}

