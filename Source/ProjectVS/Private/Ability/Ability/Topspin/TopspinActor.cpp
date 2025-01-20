// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Topspin/TopspinActor.h"
#include "PaperFlipbookComponent.h"
#include "ProjectVS.h"

#include "Ability/VSAbilityFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

#include "GenericTeamAgentInterface.h"

// Sets default values
ATopspinActor::ATopspinActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PivotRoot = CreateDefaultSubobject<USceneComponent>("PivotRoot");
	SetRootComponent(PivotRoot);
	GetRootComponent()->SetUsingAbsoluteRotation(true);

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
void ATopspinActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (Sprite)
	{
		Sprite->SetLooping(false);

		Sprite->SetVisibility(false);

		Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Sprite->OnFinishedPlaying.AddDynamic(this, &ATopspinActor::OnFinishedTopspin);

		Sprite->OnComponentBeginOverlap.AddDynamic(this, &ATopspinActor::OnBeginOverlapTopspin);
	}
}

// Called every frame
void ATopspinActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATopspinActor::StartTopspin()
{
	Sprite->SetVisibility(true);
	Sprite->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sprite->PlayFromStart();
}

void ATopspinActor::SetTopspinScale(float Scale)
{
	SetActorScale3D(FVector(Scale, 1, Scale));
}

void ATopspinActor::OnFinishedTopspin()
{
	--TopspinCount;

	if (TopspinCount <= 0)
	{
		Destroy();
	}
	else
	{
		Sprite->SetVisibility(false);

		Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetWorld()->GetTimerManager().SetTimer(TopspinTimerHandle, this, &ATopspinActor::StartTopspin, TopspinInterval, false);
	}
}

void ATopspinActor::OnBeginOverlapTopspin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!DamageEffectSpecHandle.IsValid()) return;

	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OtherActor);
	if (ASI == nullptr) return;

	UAbilitySystemComponent* OtherASC = ASI->GetAbilitySystemComponent();

	ETeamAttitude::Type Attitude = UVSAbilityFunctionLibrary::GetTeamAttitude(DamageEffectSpecHandle.Data->GetEffectContext().GetInstigatorAbilitySystemComponent(), OtherASC);
	if (Attitude != ETeamAttitude::Hostile) return;

	OtherASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data);

}

void ATopspinActor::SetTopspinCount(int NewCount)
{
	TopspinCount = NewCount;
}

