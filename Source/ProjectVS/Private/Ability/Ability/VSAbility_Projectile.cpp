// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/VSAbility_Projectile.h"
#include "Ability/Projectile/AbilityProjectileActor.h"
#include "Ability/Effect/DamageEffect.h"
#include "Ability/AttributeSet_Default.h"
#include "AbilitySystemGlobals.h"
#include "ProjectVS.h"

UVSAbility_Projectile::UVSAbility_Projectile(const FObjectInitializer& ObjectInitializer)
	: UVSAbility(ObjectInitializer)
{

	SetAbilityType(EVSAbilityType::Active);
}

float UVSAbility_Projectile::GetProjectileSpeed() const
{
	return ProjectileSpeed;
}

float UVSAbility_Projectile::GetProjectileLifeTime() const
{
	return ProjectileLifeTime;
}

float UVSAbility_Projectile::GetProjectileDamage() const
{
	return ProjectileDamage;
}

#if WITH_EDITOR
void UVSAbility_Projectile::CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const
{
	Super::CreateAbilityDataToSave(InMap);


}

void UVSAbility_Projectile::NativeLoadDataFromDataManager()
{
	Super::NativeLoadDataFromDataManager();
}

#endif

void UVSAbility_Projectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}
	AActor* Avatar = ActorInfo->AvatarActor.Get();

	FVector AvatarForward = Avatar->GetActorForwardVector();

	FTransform ProjectileTransform = Avatar->GetActorTransform();
	ProjectileTransform.SetScale3D(FVector::One());
	if (AAbilityProjectileActor* SpawnedProjectile = Avatar->GetWorld()->SpawnActorDeferred<AAbilityProjectileActor>(ProjectileClass, ProjectileTransform, Avatar))
	{
		float NewProjectileLifeTime = GetProjectileLifeTime();
		SpawnedProjectile->SetProjectileLifeTime(NewProjectileLifeTime);

		float NewProjectileSpeed = GetProjectileSpeed();
		SpawnedProjectile->SetProjectileSpeed(NewProjectileSpeed);

		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, UDamageEffect::StaticClass(), 1);
		SpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Default::GetDamageName(), ProjectileDamage);

		SpawnedProjectile->SetEffectSpecHandle(SpecHandle);

		SpawnedProjectile->FinishSpawning(ProjectileTransform);
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}


