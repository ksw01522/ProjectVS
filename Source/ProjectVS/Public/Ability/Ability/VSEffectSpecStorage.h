// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayEffectTypes.h"
#include "VSEffectSpecStorage.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVSEffectSpecStorage : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTVS_API IVSEffectSpecStorage
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
private:
	TMap<FName, FGameplayEffectSpecHandle> EffectSpecMap;

public:
	FGameplayEffectSpecHandle FindEffectSpec(const FName& InKey) const;

	void AddEffectSpec(const FName& InKey, const FGameplayEffectSpecHandle& InSpecHandle);
	void RemoveEffectSpec(const FName& InKey);
};
