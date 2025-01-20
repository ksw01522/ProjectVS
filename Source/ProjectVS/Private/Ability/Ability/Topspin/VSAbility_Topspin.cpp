// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Topspin/VSAbility_Topspin.h"
#include "AbilityDataManager.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet_Player.h"
#include "Ability/Effect/DamageEffect.h"
#include "AttributeSet_Default.h"

#include "ProjectVS.h"
#include "TopspinActor.h"

UVSAbility_Topspin::UVSAbility_Topspin(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetAbilityType(EVSAbilityType::Active);
}

void UVSAbility_Topspin::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	
	if (AvatarActor == nullptr || !CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}


	UAbilitySystemComponent* InASC = ActorInfo->AbilitySystemComponent.Get();

	int FinalTopspinCount = GetTopspinCount(Handle, InASC);

	UWorld* World = AvatarActor->GetWorld();

	float FinalScale = GetTopspinScale(Handle, InASC);

	FTransform SpawnTranform;
	SpawnTranform.SetLocation(AvatarActor->GetActorLocation());

	ATopspinActor* TopspinActor = World->SpawnActorDeferred<ATopspinActor>(TopspinActorClass, SpawnTranform);
	if (TopspinActor)
	{
		FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, false);

		TopspinActor->AttachToActor(AvatarActor, AttachRule);

		TopspinActor->SetTopspinCount(GetTopspinCount(Handle, InASC));

		FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, UDamageEffect::StaticClass());
		DamageSpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Default::GetDamageName(), GetTopspinDamage(Handle, InASC));

		TopspinActor->SetDamageEffectSpecHandle(DamageSpecHandle);

		TopspinActor->SetTopspinScale(FinalScale);
		
		TopspinActor->FinishSpawning(SpawnTranform);
		TopspinActor->StartTopspin();
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

float UVSAbility_Topspin::GetTopspinDamage(const FGameplayAbilitySpecHandle Handle, UAbilitySystemComponent* InASC) const
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
		ReturnDamage = TopspinDamage[AbLevel - 1];
	}
#endif

	bool bResult = false;

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		ReturnDamage = ADM->FindAbilityData(TopspinDamageTag, bResult);
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

float UVSAbility_Topspin::GetTopspinScale(const FGameplayAbilitySpecHandle Handle, UAbilitySystemComponent* InASC) const
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
		if (TopspinScale.IsValidIndex(AbLevel - 1)) { RangeRate = TopspinScale[AbLevel - 1]; }
	}
#endif

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		RangeRate = ADM->FindAbilityData(TopspinScaleTag, bResult, AbLevel);
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

int UVSAbility_Topspin::GetTopspinCount(const FGameplayAbilitySpecHandle Handle, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	int AbLevel = 1;

	FGameplayAbilitySpec* Spec = InASC ? InASC->FindAbilitySpecFromHandle(Handle) : nullptr;

	if (Spec)
	{
		AbLevel = Spec->Level;
	}

	int ReturnTopspinCount = 1;
	bool bResult = false;

#if WITH_EDITOR
	if (ADM == nullptr)
	{
		if (TopspinCount.IsValidIndex(AbLevel - 1)) { ReturnTopspinCount = TopspinCount[AbLevel - 1]; }
	} 
	else{
#endif
		ReturnTopspinCount = ADM->FindAbilityData(TopspinCountTag, bResult, AbLevel);
		if (!bResult) { ReturnTopspinCount = 1; }
#if WITH_EDITOR
	}
#endif

	if (InASC != nullptr)
	{
		int ASCCount = InASC->GetGameplayAttributeValue(UAttributeSet_Player::GetProjectileCountAttribute(), bResult);
		if (bResult) { ReturnTopspinCount += ASCCount; }
	}

	return ReturnTopspinCount;
}

#if WITH_EDITOR
void UVSAbility_Topspin::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PName = PropertyChangedEvent.GetMemberPropertyName();
	if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_Topspin, TopspinDamageTag))
	{
		ApplyDataFromDataManager(TopspinDamageTag, TopspinDamage);
	}
	else if(PName == GET_MEMBER_NAME_CHECKED(UVSAbility_Topspin, TopspinScaleTag))
	{
		ApplyDataFromDataManager(TopspinScaleTag, TopspinScale);
	}
	else if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_Topspin, TopspinCountTag))
	{
		ApplyDataFromDataManager(TopspinCountTag, TopspinCount);
	}
}

void UVSAbility_Topspin::CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const
{
	Super::CreateAbilityDataToSave(InMap);

	if (TopspinDamageTag.IsValid()) { InMap.Add(TopspinDamageTag.GetTagName(), TopspinDamage); }
	if (TopspinScaleTag.IsValid()) { InMap.Add(TopspinScaleTag.GetTagName(), TopspinScale); }
	if (TopspinCountTag.IsValid()) { InMap.Add(TopspinCountTag.GetTagName(), TopspinCount); }
}

void UVSAbility_Topspin::NativeLoadDataFromDataManager()
{
	Super::NativeLoadDataFromDataManager();

	ApplyDataFromDataManager(TopspinDamageTag, TopspinDamage);
	ApplyDataFromDataManager(TopspinScaleTag, TopspinScale);
	ApplyDataFromDataManager(TopspinCountTag, TopspinCount);
}
#endif