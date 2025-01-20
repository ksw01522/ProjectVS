// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "GenericTeamAgentInterface.h"
#include "AbilityProjectileActor.generated.h"

UCLASS()
class PROJECTVS_API AAbilityProjectileActor : public AActor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UProjectileMovementComponent> MovementComponent;

	float ProjectileLifeTime;

	FGameplayEffectSpecHandle EffectSpecHandle;

public:	
	// Sets default values for this actor's properties
	AAbilityProjectileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetProjectileLifeTime(float NewTime) {ProjectileLifeTime = NewTime;}
	void SetProjectileSpeed(float NewSpeed);

	void SetEffectSpecHandle(const FGameplayEffectSpecHandle& InHandle) {EffectSpecHandle = InHandle;}

	virtual FGenericTeamId GetGenericTeamId() const override;

protected:
	UFUNCTION()
	void OnProjectileOverlap(AActor* OverlappedActor, AActor* OtherActor);
};
