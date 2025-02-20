// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/VSAbilityEffecter.h"
#include "GundanceActor.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTVS_API AGundanceActor : public AVSAbilityEffecter
{
	GENERATED_BODY()
	
public:
	AGundanceActor(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override; 

private:
	UFUNCTION()
	void OnFinishedFlipbook();

	UFUNCTION()
	void OnBeginOverlapFlipbook(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
