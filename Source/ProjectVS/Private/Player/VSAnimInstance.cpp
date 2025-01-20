// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/VSAnimInstance.h"
#include "PaperZDCharacter.h"

UVSAnimInstance::UVSAnimInstance()
{
}

void UVSAnimInstance::OnTick_Implementation(float DeltaTime)
{
	AActor* OwnerA = GetOwningActor();
	if(OwnerA == nullptr) return;

	Velocity = OwnerA->GetVelocity().Size();
}
