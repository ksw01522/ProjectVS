// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Projectile/AbilityProjectileActor.h"
#include "ProjectVS.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"


// Sets default values
AAbilityProjectileActor::AAbilityProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	MovementComponent->ProjectileGravityScale = 0;
}

// Called when the game starts or when spawned
void AAbilityProjectileActor::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AAbilityProjectileActor::OnProjectileOverlap);
}

// Called every frame
void AAbilityProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProjectileLifeTime -= DeltaTime;
	if (ProjectileLifeTime <= 0)
	{
		Destroy();
	}
}

void AAbilityProjectileActor::SetProjectileSpeed(float NewSpeed)
{
	MovementComponent->MaxSpeed = NewSpeed;
	MovementComponent->InitialSpeed = NewSpeed;


}

FGenericTeamId AAbilityProjectileActor::GetGenericTeamId() const
{
	if(!EffectSpecHandle.IsValid()) return FGenericTeamId::NoTeam;

	IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(EffectSpecHandle.Data->GetContext().GetInstigator());
	if(TeamAgent == nullptr) return FGenericTeamId::NoTeam;

	return TeamAgent->GetGenericTeamId();
}

void AAbilityProjectileActor::OnProjectileOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	check(EffectSpecHandle.IsValid());

	ETeamAttitude::Type Attitude = GetTeamAttitudeTowards(*OtherActor);
	if(Attitude != ETeamAttitude::Hostile) return;

	UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OtherActor);
	if(TargetASC == nullptr) return;

	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
}

