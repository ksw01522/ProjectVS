// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Flamethrower/VSAbility_Flamethrower.h"
#include "AbilitySystemComponent.h"
#include "Ability/Effect/DamageEffect.h"
#include "Ability/Ability/Flamethrower/FlamethrowerActor.h"
#include "Ability/AttributeSet_Default.h"
#include "Ability/AbilityDataManager.h"
#include "Ability/AttributeSet_Player.h"
#include "ProjectVS.h"

UVSAbility_Flamethrower::UVSAbility_Flamethrower(const FObjectInitializer& ObjectInitializer)
						: Super(ObjectInitializer)
{

	SetAbilityType(EVSAbilityType::Active);
}

void UVSAbility_Flamethrower::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* InASC = ActorInfo->AbilitySystemComponent.Get();
	if (InASC == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* InAvatarActor = ActorInfo->AvatarActor.Get();
	if (InAvatarActor == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	float FinalFlameDamage = GetFlameDamage(Handle, InASC);
	LOG_ERROR(TEXT("Flame Damage = %.1f"), FinalFlameDamage);

	float FinalFlameScale = GetFlameScale(Handle, InASC);
	float FinalFlameDuration = GetFlameDuration(Handle, InASC);

	if (FinalFlameDuration <= 0)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(InAvatarActor->GetActorLocation());
	SpawnTransform.SetRotation(InAvatarActor->GetActorQuat());
	SpawnTransform.SetScale3D(FVector::One() * FinalFlameScale);

	UWorld* InWorld = InAvatarActor->GetWorld();

	if (AFlamethrowerActor* NewFlamethrowerActor = InWorld->SpawnActorDeferred<AFlamethrowerActor>(FlamethrowerActorClass, SpawnTransform))
	{
		FGameplayEffectSpecHandle NewDamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, UDamageEffect::StaticClass(), 1);
		NewDamageEffectSpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Default::GetDamageName(), FinalFlameDamage * 0.25);

		NewFlamethrowerActor->SetDamageEffectSpecHandle(NewDamageEffectSpecHandle);

		NewFlamethrowerActor->SetFlameDuration(FinalFlameDuration);

		NewFlamethrowerActor->AttachToActor(InAvatarActor, FAttachmentTransformRules(EAttachmentRule::KeepWorld, false));

		NewFlamethrowerActor->FinishSpawning(SpawnTransform);

		NewFlamethrowerActor->FlameStart();
	}


	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

#if WITH_EDITOR

void UVSAbility_Flamethrower::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PName = PropertyChangedEvent.GetMemberPropertyName();
	if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_Flamethrower, FlameDamageTag))
	{
		ApplyDataFromDataManager(FlameDamageTag, FlameDamage);
	}
	else if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_Flamethrower, FlameScaleTag))
	{
		ApplyDataFromDataManager(FlameScaleTag, FlameScale);
	}
	else if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_Flamethrower, FlameDurationTag))
	{
		ApplyDataFromDataManager(FlameDurationTag, FlameDuration);
	}
}

void UVSAbility_Flamethrower::CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const
{
	Super::CreateAbilityDataToSave(InMap);

	if (FlameDamageTag.IsValid()) { InMap.Add(FlameDamageTag.GetTagName(), FlameDamage); }
	if (FlameScaleTag.IsValid()) { InMap.Add(FlameScaleTag.GetTagName(), FlameScale); }
	if (FlameDurationTag.IsValid()) { InMap.Add(FlameDurationTag.GetTagName(), FlameDuration); }
}

void UVSAbility_Flamethrower::NativeLoadDataFromDataManager()
{
	Super::NativeLoadDataFromDataManager();

	ApplyDataFromDataManager(FlameDamageTag, FlameDamage);
	ApplyDataFromDataManager(FlameScaleTag, FlameScale);
	ApplyDataFromDataManager(FlameDurationTag, FlameDuration);
}

#endif

float UVSAbility_Flamethrower::GetFlameDamage(const FGameplayAbilitySpecHandle Handle, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	int AbLevel = 1;

	FGameplayAbilitySpec* Spec = InASC ? InASC->FindAbilitySpecFromHandle(Handle) : nullptr;

	if (Spec)
	{
		AbLevel = Spec->Level;
	}

	float ReturnDamage = 0;

#if WITH_EDITOR
	if (ADM == nullptr)
	{
		ReturnDamage = FlameDamage[AbLevel];
	}
#endif

	bool bResult = false;

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		ReturnDamage = ADM->FindAbilityData(FlameDamageTag, bResult);
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

float UVSAbility_Flamethrower::GetFlameScale(const FGameplayAbilitySpecHandle Handle, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	int AbLevel = 1;

	FGameplayAbilitySpec* Spec = InASC ? InASC->FindAbilitySpecFromHandle(Handle) : nullptr;

	if (Spec)
	{
		AbLevel = Spec->Level;
	}

	float RangeRate = 1;
	bool bResult = false;


#if WITH_EDITOR
	if (ADM == nullptr)
	{
		if (FlameScale.IsValidIndex(AbLevel)) { RangeRate = FlameScale[AbLevel]; }
	}
#endif

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		RangeRate = ADM->FindAbilityData(FlameScaleTag, bResult, AbLevel);
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

float UVSAbility_Flamethrower::GetFlameDuration(const FGameplayAbilitySpecHandle Handle, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	int AbLevel = 1;

	FGameplayAbilitySpec* Spec = InASC ? InASC->FindAbilitySpecFromHandle(Handle) : nullptr;

	if (Spec)
	{
		AbLevel = Spec->Level;
	}

	float RangeRate = 1;
	bool bResult = false;


#if WITH_EDITOR
	if (ADM == nullptr)
	{
		if (FlameDuration.IsValidIndex(AbLevel)) { RangeRate = FlameDuration[AbLevel]; }
	}
#endif

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		RangeRate = ADM->FindAbilityData(FlameDurationTag, bResult, AbLevel);
		if (!bResult) RangeRate = 1;
#if WITH_EDITOR
	}
#endif

	if (InASC != nullptr)
	{
		float ABSRangeRate = InASC->GetGameplayAttributeValue(UAttributeSet_Player::GetEffectTimeRateAttribute(), bResult);
		if (bResult) { RangeRate *= ABSRangeRate; }
	}

	return RangeRate;
}
