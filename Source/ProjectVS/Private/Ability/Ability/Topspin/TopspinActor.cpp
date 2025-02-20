// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Topspin/TopspinActor.h"
#include "PaperFlipbookComponent.h"
#include "ProjectVS.h"

#include "Ability/VSAbilityFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

#include "GenericTeamAgentInterface.h"

// Sets default values
ATopspinActor::ATopspinActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	GetRootComponent()->SetUsingAbsoluteRotation(true);
}

// Called when the game starts or when spawned
void ATopspinActor::BeginPlay()
{
	Super::BeginPlay();
	
	UPaperFlipbookComponent* Flipbook = GetFlipbookComponent();

	if (Flipbook)
	{
		Flipbook->SetLooping(false);

		Flipbook->SetVisibility(false);

		Flipbook->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Flipbook->OnFinishedPlaying.AddDynamic(this, &ATopspinActor::OnFinishedTopspin);

		Flipbook->OnComponentBeginOverlap.AddDynamic(this, &ATopspinActor::OnBeginOverlapTopspin);
	}
}

// Called every frame
void ATopspinActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATopspinActor::StartTopspin()
{
	UPaperFlipbookComponent* Flipbook = GetFlipbookComponent();

	Flipbook->SetVisibility(true);
	Flipbook->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Flipbook->PlayFromStart();
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
		UPaperFlipbookComponent* Flipbook = GetFlipbookComponent();

		Flipbook->SetVisibility(false);

		Flipbook->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetWorld()->GetTimerManager().SetTimer(TopspinTimerHandle, this, &ATopspinActor::StartTopspin, TopspinInterval, false);
	}
}

void ATopspinActor::OnBeginOverlapTopspin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FGameplayEffectSpecHandle DamageEffectSpecHandle = FindEffectSpec("TopspinDamage");
	check(DamageEffectSpecHandle.IsValid())

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

