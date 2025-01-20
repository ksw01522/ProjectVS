// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "CooldownEffect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVS_API UCooldownEffect : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UCooldownEffect();

	static FName CoolTimeSetByCallerName;
};
