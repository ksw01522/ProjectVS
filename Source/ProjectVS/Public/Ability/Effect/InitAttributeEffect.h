// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "InitAttributeEffect.generated.h"

#define ADD_ATTRIBUTE_MODIFER(ClassName, PropertyName) \
{\
	FGameplayModifierInfo InfoToAdd;\
	\
	InfoToAdd.Attribute = ClassName::Get##PropertyName##Attribute(); \
	InfoToAdd.ModifierOp = EGameplayModOp::Override; \
	\
	FSetByCallerFloat Caller; \
	\
	Caller.DataName = ClassName::Get##PropertyName##Name(); \
	\
	InfoToAdd.ModifierMagnitude = FGameplayEffectModifierMagnitude(Caller); \
	Modifiers.Add(InfoToAdd); \
}

/**
 * 
 */
UCLASS()
class PROJECTVS_API UInitAttributeEffect : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UInitAttributeEffect();
};
