// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/VSEffectSpecStorage.h"

// Add default functionality here for any IVSEffectSpecStorage functions that are not pure virtual.

FGameplayEffectSpecHandle IVSEffectSpecStorage::FindEffectSpec(const FName& InKey) const
{
	if(!EffectSpecMap.Contains(InKey)) return FGameplayEffectSpecHandle();

	return EffectSpecMap[InKey];
}

void IVSEffectSpecStorage::AddEffectSpec(const FName& InKey, const FGameplayEffectSpecHandle& InSpecHandle)
{
	if(InKey.IsNone() || !InSpecHandle.IsValid()) return;

	EffectSpecMap.Add(InKey, InSpecHandle);
}

void IVSEffectSpecStorage::RemoveEffectSpec(const FName& InKey)
{
	EffectSpecMap.Remove(InKey);
}
