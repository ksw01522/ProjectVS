// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ability/Ability/VSAbilityEffecter.h"
#include "GameplayEffectTypes.h"
#include "FlamethrowerActor.generated.h"

UCLASS(Abstract)
class PROJECTVS_API AFlamethrowerActor : public AVSAbilityEffecter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlamethrowerActor(const FObjectInitializer& ObjectInitiailzer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:


	UPROPERTY(Category = Flipbook, EditAnywhere, meta = (DisplayThumbnail = "true"))
	TObjectPtr<class UPaperFlipbook> Flipbook_Start;

	UPROPERTY(Category = Flipbook, EditAnywhere, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> Flipbook_Loop;

	UPROPERTY(Category = Flipbook, EditAnywhere, meta = (DisplayThumbnail = "true"))
	TObjectPtr<UPaperFlipbook> Flipbook_End;

private:

	UFUNCTION()
	void OnFinishedFlameStartAnimation();

	FTimerHandle TimerHandle_FlameTick;

	void FlameTick();

	FTimerHandle TimerHandle_FlameEnd;

	void FlameEnd();

	float FlameDuration = 0;

public:
	void SetFlameDuration(float NewTime);

	void FlameStart();
};
