// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "VSAbilityFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVS_API UVSAbilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	

public:
	static ETeamAttitude::Type GetTeamAttitude(class AActor* A, AActor* B);

	static ETeamAttitude::Type GetTeamAttitude(class UAbilitySystemComponent* A, AActor* B);

	static ETeamAttitude::Type GetTeamAttitude(AActor* A, UAbilitySystemComponent* B);

	static ETeamAttitude::Type GetTeamAttitude(UAbilitySystemComponent* A, UAbilitySystemComponent* B);
};
