// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#define GAMEPLAYATTRIBUTE_TAG_GETTER(PropertyName) \
	FORCEINLINE static FGameplayTag Get##PropertyName##Tag() \
	{ \
		FString Category = ATTRIBUTE_CATEGORY; \
		FString PName = #PropertyName; \
		return FGameplayTag::RequestGameplayTag(FName(FString::Printf(TEXT("Attribute.%s.%s"), *Category, *PName) ) ); \
	}

#define GAMEPLAYATTRIBUTE_NAME_GETTER(PropertyName) \
FORCEINLINE static FName Get##PropertyName##Name() \
{\
	return FName(#PropertyName);\
}

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) \
	GAMEPLAYATTRIBUTE_NAME_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_TAG_GETTER(PropertyName)
