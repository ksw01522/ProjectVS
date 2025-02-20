// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/VSAbilityEffecter.h"
#include "TopspinActor.generated.h"

UCLASS()
class PROJECTVS_API ATopspinActor : public AVSAbilityEffecter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATopspinActor(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TopspinInterval = 0.25f;

	int TopspinCount = 0;

	FTimerHandle TopspinTimerHandle;

private:

	UFUNCTION()
	void OnFinishedTopspin();

	UFUNCTION()
	void OnBeginOverlapTopspin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void SetTopspinCount(int NewCount);
	int GetTopspinCount() const { return TopspinCount;}

	void StartTopspin();

	void SetTopspinScale(float Scale);
};
