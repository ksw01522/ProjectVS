// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/CannonBall/VSAbility_CannonBall.h"
#include "CannonBallActor.h"
#include "AbilityDataManager.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet_Player.h"
#include "Ability/Effect/DamageEffect.h"
#include "AttributeSet_Default.h"

#include "ProjectVS.h"

#include "Character/Monster/MonsterWorldManager.h"

#include "Character/Monster/VSMonster.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif
UVSAbility_CannonBall::UVSAbility_CannonBall(const FObjectInitializer& ObjectInitailizer)
	:Super(ObjectInitailizer)
{
	SetAbilityType(EVSAbilityType::Active);
}
void UVSAbility_CannonBall::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	UAbilitySystemComponent* InASC = ActorInfo->AbilitySystemComponent.Get();

	if (AvatarActor == nullptr || InASC == nullptr || !CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	int AbLevel = GetAbilityLevel(Handle, ActorInfo);

	float FinalCannonBallDamage = GetCannonBallDamage(AbLevel, InASC);
	float FinalExplosionDamage = GetExplosionDamage(AbLevel, InASC);
	float FinalCannonBallScale = GetCannonBallScale(AbLevel, InASC);
	FQuat CannonBallRotation = CreateCannonBallRoation(AvatarActor);
	float FinalCannonBallSpeed = GetCannonBallSpeed(AbLevel, InASC);

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(AvatarActor->GetActorLocation());
	SpawnTransform.SetScale3D(FVector(FinalCannonBallScale, FinalCannonBallScale, 1));
	SpawnTransform.SetRotation(CannonBallRotation);


	ACannonBallActor* CannonBallActor = AvatarActor->GetWorld()->SpawnActorDeferred<ACannonBallActor>(CannonBallActorClass, SpawnTransform);
	if (CannonBallActor)
	{
		FGameplayEffectSpecHandle CannonBallDamageSpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, UDamageEffect::StaticClass());
		CannonBallDamageSpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Default::GetDamageName(), FinalCannonBallDamage);

		FGameplayEffectSpecHandle ExplosionDamageSpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, UDamageEffect::StaticClass());
		ExplosionDamageSpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Default::GetDamageName(), FinalExplosionDamage);

		CannonBallActor->AddEffectSpec("CannonBallDamage", CannonBallDamageSpecHandle);
		CannonBallActor->AddEffectSpec("ExplosionDamage", ExplosionDamageSpecHandle);

		FVector BallVelocity = CannonBallRotation.GetForwardVector() * FinalCannonBallSpeed;
		CannonBallActor->SetCannonBallVelocity(BallVelocity);


		CannonBallActor->FinishSpawning(SpawnTransform);
	}


	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

FQuat UVSAbility_CannonBall::CreateCannonBallRoation(AActor* AvatarActor) const
{
	if (AvatarActor == nullptr) { return FQuat::Identity; }

	FVector AvatarLocation = AvatarActor->GetActorLocation();

	AVSMonster* Target = nullptr;

	TArray<AVSMonster*> Monsters;

	UMonsterWorldManager* MWM = AvatarActor->GetWorld()->GetSubsystem<UMonsterWorldManager>();

	if (MWM == nullptr) { return FQuat::Identity; }

	MWM->GetMonsters(Monsters);

	if (!Monsters.IsEmpty())
	{
		Target = Monsters[0];
		float MinDist = FVector::DistXY(AvatarLocation, Target->GetActorLocation());
		float TempDist = 0;

		int MaxIdx = Monsters.Num();

		for (int i = 1; i < MaxIdx; i++)
		{
			TempDist = FVector::DistXY(AvatarLocation, Monsters[i]->GetActorLocation());

			if (TempDist < MinDist)
			{
				Target = Monsters[i];
				MinDist = TempDist;
			}
		}
	}

	if (Target == nullptr) return AvatarActor->GetActorQuat();

	FVector TargetLocation = Target->GetActorLocation();

	FVector Dir = TargetLocation - AvatarLocation;
	Dir.Z = 0;

	return Dir.ToOrientationQuat();
}

float UVSAbility_CannonBall::GetCannonBallDamage(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	float ReturnDamage = 0;

#if WITH_EDITOR
	if (ADM == nullptr)
	{
		ReturnDamage = CannonBallDamage[InLevel];
	}
#endif

	bool bResult = false;

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		ReturnDamage = ADM->FindAbilityData(CannonBallDamageTag, bResult, InLevel);
#if WITH_EDITOR
	}
#endif

	if (InASC != nullptr)
	{
		float DamageRate = InASC->GetGameplayAttributeValue(UAttributeSet_Player::GetDamageRateAttribute(), bResult);
		if (bResult)
		{
			ReturnDamage *= DamageRate;
		}
	}

	return ReturnDamage;
}

float UVSAbility_CannonBall::GetExplosionDamage(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	float ReturnDamage = 0;

#if WITH_EDITOR
	if (ADM == nullptr)
	{
		ReturnDamage = ExplosionDamage[InLevel];
	}
#endif

	bool bResult = false;

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		ReturnDamage = ADM->FindAbilityData(ExplosionDamageTag, bResult, InLevel);
#if WITH_EDITOR
	}
#endif

	if (InASC != nullptr)
	{
		float DamageRate = InASC->GetGameplayAttributeValue(UAttributeSet_Player::GetDamageRateAttribute(), bResult);
		if (bResult)
		{
			ReturnDamage *= DamageRate;
		}
	}

	return ReturnDamage;
}

float UVSAbility_CannonBall::GetCannonBallScale(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	float RangeRate = 1;
	bool bResult = false;


#if WITH_EDITOR
	if (ADM == nullptr)
	{
		if (CannonBallScale.IsValidIndex(InLevel)) { RangeRate = CannonBallScale[InLevel]; }
	}
#endif

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		RangeRate = ADM->FindAbilityData(CannonBallScaleTag, bResult, InLevel);
		if (!bResult) RangeRate = 1;
#if WITH_EDITOR
	}
#endif

	if (InASC != nullptr)
	{
		float ABSRangeRate = InASC->GetGameplayAttributeValue(UAttributeSet_Player::GetEffectRangeRateAttribute(), bResult);
		if (bResult) { RangeRate *= ABSRangeRate; }
	}

	return RangeRate;
}

float UVSAbility_CannonBall::GetCannonBallSpeed(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	float ReturnSpeed = 1;
	bool bResult = false;


#if WITH_EDITOR
	if (ADM == nullptr)
	{
		if (CannonBallSpeed.IsValidIndex(InLevel)) { ReturnSpeed = CannonBallSpeed[InLevel]; }
	}
#endif

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		ReturnSpeed = ADM->FindAbilityData(CannonBallSpeedTag, bResult, InLevel);
		if (!bResult) ReturnSpeed = 1;
#if WITH_EDITOR
	}
#endif


	return ReturnSpeed;
}

void UVSAbility_CannonBall::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PName = PropertyChangedEvent.GetMemberPropertyName();
	if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_CannonBall, CannonBallDamageTag))
	{
		ApplyDataFromDataManager(CannonBallDamageTag, CannonBallDamage);
	}
	else if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_CannonBall, CannonBallScaleTag))
	{
		ApplyDataFromDataManager(CannonBallScaleTag, CannonBallScale);
	}
	else if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_CannonBall, ExplosionDamageTag))
	{
		ApplyDataFromDataManager(ExplosionDamageTag, ExplosionDamage);
	}
	else if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_CannonBall, CannonBallSpeedTag))
	{
		ApplyDataFromDataManager(CannonBallSpeedTag, CannonBallSpeed);
	}

}

void UVSAbility_CannonBall::CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const
{
	Super::CreateAbilityDataToSave(InMap);

	if (CannonBallDamageTag.IsValid()) { InMap.Add(CannonBallDamageTag.GetTagName(), CannonBallDamage); }
	if (CannonBallScaleTag.IsValid()) { InMap.Add(CannonBallScaleTag.GetTagName(), CannonBallScale); }
}

void UVSAbility_CannonBall::NativeLoadDataFromDataManager()
{
	Super::NativeLoadDataFromDataManager();

	ApplyDataFromDataManager(CannonBallDamageTag, CannonBallDamage);
	ApplyDataFromDataManager(CannonBallScaleTag, CannonBallScale);
}
