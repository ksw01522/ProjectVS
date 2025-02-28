// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Effect/DamageEffect.h"
#include "Ability/Effect/DamageEffectExecutionCalculation.h"
#include "Ability/AttributeSet_Default.h"

UDamageEffect::UDamageEffect()
{
	FGameplayEffectExecutionDefinition EED;
	EED.CalculationClass = UDamageEffectExecutionCalculation::StaticClass();

	FGameplayEffectExecutionScopedModifierInfo EESMI(UAttributeSet_Default::GetDamageTag());

	//데미지 추가
	FSetByCallerFloat DamageCaller;
	DamageCaller.DataName = UAttributeSet_Default::GetDamageName();

	EESMI.ModifierMagnitude = FGameplayEffectModifierMagnitude(DamageCaller);

	//죽은 상태일 떄 무시하는 거 추가
	EESMI.TargetTags.IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("State.Dead"));

	EED.CalculationModifiers.Add(EESMI);

	Executions.Add(EED);
}

FGameplayEffectSpecHandle UDamageEffect::MakeDamageEffectSpec(float NewDamage, const FGameplayEffectContextHandle& ContextHandle, TSubclassOf<UDamageEffect> DamageClass)
{
	if(!ContextHandle.IsValid()) return FGameplayEffectSpecHandle();
	
	FGameplayEffectSpecHandle SpecHandle(new FGameplayEffectSpec(DamageClass.GetDefaultObject(), ContextHandle));
	FGameplayEffectSpec* Spec = SpecHandle.Data.Get();

	Spec->SetSetByCallerMagnitude(UAttributeSet_Default::GetDamageTag(), NewDamage);

	return SpecHandle;
}
