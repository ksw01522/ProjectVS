// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/HandCannon/HandCannonFireActor.h"
#include "PaperFlipbookComponent.h"
#include "ProjectVS.h"

#include "Ability/VSAbilityFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

#include "GenericTeamAgentInterface.h"


// Sets default values
AHandCannonFireActor::AHandCannonFireActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	PivotRoot = CreateDefaultSubobject<USceneComponent>("PivotRoot");
	SetRootComponent(PivotRoot);

	Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>("Sprite");
	if (Sprite)
	{
		Sprite->AlwaysLoadOnClient = true;
		Sprite->AlwaysLoadOnServer = true;
		Sprite->bOwnerNoSee = false;
		Sprite->bAffectDynamicIndirectLighting = false;
		Sprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Sprite->SetupAttachment(PivotRoot);
		Sprite->SetGenerateOverlapEvents(false);

		Sprite->SetLooping(true);
	}
}

// Called when the game starts or when spawned
void AHandCannonFireActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (Sprite)
	{
		Sprite->SetLooping(false);

		Sprite->OnFinishedPlaying.AddDynamic(this, &AHandCannonFireActor::OnFinishedFire);

		Sprite->OnComponentBeginOverlap.AddDynamic(this, &AHandCannonFireActor::OnBeginOverlapFire);
	}
}

// Called every frame
void AHandCannonFireActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHandCannonFireActor::OnFinishedFire()
{

	Destroy();
}

void AHandCannonFireActor::OnBeginOverlapFire(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!DamageEffectSpecHandle.IsValid()) return;

	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OtherActor);
	if(ASI == nullptr) return;

	UAbilitySystemComponent* OtherASC = ASI->GetAbilitySystemComponent();

	ETeamAttitude::Type Attitude = UVSAbilityFunctionLibrary::GetTeamAttitude(DamageEffectSpecHandle.Data->GetEffectContext().GetInstigatorAbilitySystemComponent(), OtherASC);
	if(Attitude != ETeamAttitude::Hostile) return;

	OtherASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data);
}

void AHandCannonFireActor::SetFireSize(float NewSize)
{
	SetActorScale3D(FVector(NewSize, NewSize, 1));
}

