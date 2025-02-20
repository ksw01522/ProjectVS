// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/VSAbilityEffecter.h"
#include "PaperFlipbookComponent.h"


// Sets default values
AVSAbilityEffecter::AVSAbilityEffecter(const FObjectInitializer& ObjectInitalizer)
	: Super(ObjectInitalizer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PivotRoot = CreateDefaultSubobject<USceneComponent>("PivotRoot");
	SetRootComponent(PivotRoot);

	FlipbookComponent = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>("FlipbookComponent");
	if (FlipbookComponent)
	{
		FlipbookComponent->AlwaysLoadOnClient = true;
		FlipbookComponent->AlwaysLoadOnServer = true;
		FlipbookComponent->bOwnerNoSee = false;
		FlipbookComponent->bAffectDynamicIndirectLighting = false;
		FlipbookComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		FlipbookComponent->SetupAttachment(PivotRoot);
		FlipbookComponent->SetGenerateOverlapEvents(false);

		FlipbookComponent->SetLooping(true);

		FlipbookComponent->SetRelativeRotation(FRotator(0,0,-90));

		FlipbookComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(EWalkableSlopeBehavior::WalkableSlope_Unwalkable, 30));
		FlipbookComponent->SetCollisionProfileName("OverlapAllDynamic", false);
		FlipbookComponent->SetGenerateOverlapEvents(true);
	}
}



// Called when the game starts or when spawned
void AVSAbilityEffecter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVSAbilityEffecter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

