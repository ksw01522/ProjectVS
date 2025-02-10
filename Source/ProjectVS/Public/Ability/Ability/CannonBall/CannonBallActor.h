// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ability/Ability/VSEffectSpecStorage.h"
#include "CannonBallActor.generated.h"

UCLASS(Abstract)
class PROJECTVS_API ACannonBallActor : public AActor, public IVSEffectSpecStorage
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannonBallActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(Category = Topspin, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> PivotRoot;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbookComponent> Sprite;

	UPROPERTY(Category = Flipbook, EditAnywhere, meta = (DisplayThumbnail = "true"))
	TObjectPtr<class UPaperFlipbook> Flipbook_Explosion;


	UPROPERTY(Transient)
	FVector Velocity;

	UPROPERTY(EditDefaultsOnly, Category = "CannonBall", Transient)
	float MaxDistance = 0;

	float RemainDistance = 0;

	FVector DefaultScale;

public:
	void SetCannonBallVelocity(const FVector& NewVelocity){Velocity = NewVelocity;}

private:
	UFUNCTION()
	void OnBeginOverlapCannonBall(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBeginOverlapExplosion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	void CannonBallExplosion();

	void CannonBallAnim();
};
