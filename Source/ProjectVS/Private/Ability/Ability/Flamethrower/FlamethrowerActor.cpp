// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Flamethrower/FlamethrowerActor.h"
#include "PaperFlipbookComponent.h"
#include "ProjectVS.h"

#include "Ability/VSAbilityFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

#include "GenericTeamAgentInterface.h"
#include "Engine/OverlapResult.h"
#include "PaperFlipbook.h"

// Sets default values
AFlamethrowerActor::AFlamethrowerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PivotRoot = CreateDefaultSubobject<USceneComponent>("Pivot");
	SetRootComponent(PivotRoot);
	PivotRoot->SetUsingAbsoluteRotation(true);
	PivotRoot->SetUsingAbsoluteScale(true);

	FlameFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>("FlameFlipbook");
	FlameFlipbook->SetupAttachment(PivotRoot);
	FlameFlipbook->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AFlamethrowerActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFlamethrowerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFlamethrowerActor::FlameStart()
{
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	TimerManager.SetTimer(TimerHandle_FlameEnd, this, &AFlamethrowerActor::FlameEnd, FlameDuration, false);

	FlameFlipbook->SetFlipbook(Flipbook_Start);
	FlameFlipbook->SetLooping(false);

	FlameFlipbook->OnFinishedPlaying.AddDynamic(this, &AFlamethrowerActor::OnFinishedFlameStartAnimation);

	FlameFlipbook->PlayFromStart();

	static float FlameTickinterval = 0.25;
	TimerManager.SetTimer(TimerHandle_FlameTick, this, &AFlamethrowerActor::FlameTick, FlameTickinterval, true);
}

void AFlamethrowerActor::OnFinishedFlameStartAnimation()
{
	FlameFlipbook->OnFinishedPlaying.RemoveDynamic(this, &AFlamethrowerActor::OnFinishedFlameStartAnimation);

	FlameFlipbook->SetFlipbook(Flipbook_Loop);
	FlameFlipbook->SetLooping(true);
	FlameFlipbook->PlayFromStart();
}

void AFlamethrowerActor::FlameTick()
{
	if(!DamageEffectSpecHandle.IsValid()) return;

	TArray<FOverlapResult> OverlapResults;
	
	FComponentQueryParams QueryParams("Flamethrower Overlap", this);
	AActor* AvatarActor = DamageEffectSpecHandle.Data->GetEffectContext().GetInstigatorAbilitySystemComponent()->GetAvatarActor();

	QueryParams.AddIgnoredActor(AvatarActor);

	if (GetWorld()->ComponentOverlapMulti(OverlapResults, FlameFlipbook, FlameFlipbook->GetComponentLocation(), FlameFlipbook->GetComponentQuat(), QueryParams))
	{
		for (auto& OverlapResult : OverlapResults)
		{
			AActor* OverlappedActor = OverlapResult.GetActor();

			IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OverlappedActor);
			if (ASI == nullptr) return;

			UAbilitySystemComponent* OtherASC = ASI->GetAbilitySystemComponent();

			ETeamAttitude::Type Attitude = UVSAbilityFunctionLibrary::GetTeamAttitude(DamageEffectSpecHandle.Data->GetEffectContext().GetInstigatorAbilitySystemComponent(), OtherASC);
			if (Attitude != ETeamAttitude::Hostile) return;

			OtherASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data);

			LOG_ERROR(TEXT("%s"), *OverlappedActor->GetName());
		}
	}

	
}

void AFlamethrowerActor::FlameEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_FlameTick);

	if (FlameFlipbook)
	{
		FlameFlipbook->SetFlipbook(Flipbook_End);

		FlameFlipbook->SetLooping(false);

		FlameFlipbook->PlayFromStart();
	}

	float EndDuration = Flipbook_End->GetTotalDuration();

	SetLifeSpan(EndDuration);
}

void AFlamethrowerActor::SetFlameDuration(float NewTime)
{
	FlameDuration = NewTime;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(TimerHandle_FlameEnd))
	{
		TimerManager.SetTimer(TimerHandle_FlameEnd, this, &AFlamethrowerActor::FlameEnd, FlameDuration, false);
	}
}

