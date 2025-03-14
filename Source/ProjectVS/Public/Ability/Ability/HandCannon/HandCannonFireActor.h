// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/VSAbilityEffecter.h"
#include "GameplayEffectTypes.h"
#include "HandCannonFireActor.generated.h"

UCLASS(Abstract)
class PROJECTVS_API AHandCannonFireActor : public AVSAbilityEffecter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHandCannonFireActor(const FObjectInitializer& ObjectInitializer);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UFUNCTION()
	void OnFinishedFire();

	UFUNCTION()
	void OnBeginOverlapFire(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void SetFireSize(float NewSize);
};
