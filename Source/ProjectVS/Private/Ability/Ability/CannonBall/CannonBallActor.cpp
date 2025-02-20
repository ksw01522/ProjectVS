// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/CannonBall/CannonBallActor.h"
#include "PaperFlipbookComponent.h"
#include "ProjectVS.h"

#include "Ability/VSAbilityFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

#include "GenericTeamAgentInterface.h"

// Sets default values
ACannonBallActor::ACannonBallActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void ACannonBallActor::BeginPlay()
{
	Super::BeginPlay();
	
	RemainDistance = MaxDistance;
	GetFlipbookComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACannonBallActor::OnBeginOverlapCannonBall);

}

// Called every frame
void ACannonBallActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Velocity.IsNearlyZero()) 
	{
		FVector Offset = Velocity * DeltaTime;
		AddActorWorldOffset(Offset);

		RemainDistance -= Offset.Size();

		if (RemainDistance <= 0)
		{
			CannonBallExplosion();
		}
	}
}

void ACannonBallActor::OnBeginOverlapCannonBall(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FGameplayEffectSpecHandle DamageSpecHandle = FindEffectSpec("CannonBallDamage");

	if (!DamageSpecHandle.IsValid()) return;

	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OtherActor);
	if (ASI == nullptr) return;

	UAbilitySystemComponent* OtherASC = ASI->GetAbilitySystemComponent();

	ETeamAttitude::Type Attitude = UVSAbilityFunctionLibrary::GetTeamAttitude(DamageSpecHandle.Data->GetEffectContext().GetInstigatorAbilitySystemComponent(), OtherASC);
	if (Attitude != ETeamAttitude::Hostile) return;

	OtherASC->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data);
}

void ACannonBallActor::OnBeginOverlapExplosion(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FGameplayEffectSpecHandle DamageSpecHandle = FindEffectSpec("ExplosionDamage");

	if (!DamageSpecHandle.IsValid()) return;

	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OtherActor);
	if (ASI == nullptr) return;

	UAbilitySystemComponent* OtherASC = ASI->GetAbilitySystemComponent();

	ETeamAttitude::Type Attitude = UVSAbilityFunctionLibrary::GetTeamAttitude(DamageSpecHandle.Data->GetEffectContext().GetInstigatorAbilitySystemComponent(), OtherASC);
	if (Attitude != ETeamAttitude::Hostile) return;

	OtherASC->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data);

}

void ACannonBallActor::CannonBallExplosion()
{
	Velocity = FVector::ZeroVector;

	UPaperFlipbookComponent* Flipbook = GetFlipbookComponent();

	Flipbook->OnComponentBeginOverlap.RemoveDynamic(this, &ACannonBallActor::OnBeginOverlapCannonBall);

	Flipbook->OnComponentBeginOverlap.AddDynamic(this, &ACannonBallActor::OnBeginOverlapExplosion);

	Flipbook->SetFlipbook(Flipbook_Explosion);

	DefaultScale = GetActorScale3D();
	SetActorScale3D(FVector(0.25,0.25,0.25));

	SetLifeSpan(0.25);
	CannonBallAnim();
}

void ACannonBallActor::CannonBallAnim()
{
	float DeltaSeconds = GetWorld()->GetDeltaSeconds();

	SetActorScale3D(GetActorScale3D() + DefaultScale * DeltaSeconds * 8);
	if(DefaultScale.X < GetActorScale3D().X) {SetActorScale3D(DefaultScale); }

	UPaperFlipbookComponent* Flipbook = GetFlipbookComponent();

	FLinearColor CurrentColor = Flipbook->GetSpriteColor();
	
	CurrentColor.A -= 4 * DeltaSeconds;

	if (CurrentColor.A <= 0) CurrentColor.A = 0;

	Flipbook->SetSpriteColor(CurrentColor);
	
	if (0 < CurrentColor.A)
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ACannonBallActor::CannonBallAnim);
	}
}
