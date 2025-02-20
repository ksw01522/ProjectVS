// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ability/Ability/VSEffectSpecStorage.h"
#include "VSAbilityEffecter.generated.h"

UCLASS()
class PROJECTVS_API AVSAbilityEffecter : public AActor, public IVSEffectSpecStorage
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVSAbilityEffecter(const FObjectInitializer& ObjectInitalizer);

private:
	UPROPERTY(Category = Topspin, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> PivotRoot;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPaperFlipbookComponent> FlipbookComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPaperFlipbookComponent* GetFlipbookComponent() const {return FlipbookComponent;}
};
