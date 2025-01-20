// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effect/InitAttributeEffect.h"
#include "Ability/AttributeSet_Default.h"




UInitAttributeEffect::UInitAttributeEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Default, Level);
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Default, MaxHP);
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Default, MoveSpeed);
}
