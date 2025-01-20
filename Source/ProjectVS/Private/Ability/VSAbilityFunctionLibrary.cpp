// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/VSAbilityFunctionLibrary.h"
#include "AbilitySystemComponent.h"

ETeamAttitude::Type UVSAbilityFunctionLibrary::GetTeamAttitude(AActor* A, AActor* B)
{
	if(A == nullptr || B == nullptr) return ETeamAttitude::Neutral;

	return FGenericTeamId::GetAttitude(A, B);
}

ETeamAttitude::Type UVSAbilityFunctionLibrary::GetTeamAttitude(UAbilitySystemComponent* A, AActor* B)
{
	AActor* ActorA = A == nullptr ? nullptr : A->GetOwnerActor();

	return GetTeamAttitude(ActorA, B);
}

ETeamAttitude::Type UVSAbilityFunctionLibrary::GetTeamAttitude(AActor* A, UAbilitySystemComponent* B)
{
	AActor* ActorB = B == nullptr ? nullptr : B->GetOwnerActor();

	return GetTeamAttitude(A, ActorB);
}

ETeamAttitude::Type UVSAbilityFunctionLibrary::GetTeamAttitude(UAbilitySystemComponent* A, UAbilitySystemComponent* B)
{
	AActor* ActorA = A == nullptr ? nullptr : A->GetOwnerActor();
	AActor* ActorB = B == nullptr ? nullptr : B->GetOwnerActor();

	return GetTeamAttitude(ActorA, ActorB);
}
