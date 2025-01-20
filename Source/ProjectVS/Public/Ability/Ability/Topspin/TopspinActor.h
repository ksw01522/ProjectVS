// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "TopspinActor.generated.h"

UCLASS()
class PROJECTVS_API ATopspinActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATopspinActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(Category = Topspin, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> PivotRoot;

	UPROPERTY(Category = Topspin, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbookComponent> Sprite;

	UPROPERTY(Category = Sprite, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float TopspinInterval = 0.25f;

	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	int TopspinCount = 0;

	FTimerHandle TopspinTimerHandle;

private:

	UFUNCTION()
	void OnFinishedTopspin();

	UFUNCTION()
	void OnBeginOverlapTopspin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	void SetDamageEffectSpecHandle(const FGameplayEffectSpecHandle& NewHandle) { DamageEffectSpecHandle = NewHandle; }

	void SetTopspinCount(int NewCount);
	int GetTopspinCount() const { return TopspinCount;}

	void StartTopspin();

	void SetTopspinScale(float Scale);
};
