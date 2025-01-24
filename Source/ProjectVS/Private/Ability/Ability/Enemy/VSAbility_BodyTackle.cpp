// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Enemy/VSAbility_BodyTackle.h"
#include "Ability/Effect/CooldownEffect.h"
#include "AbilitySystemComponent.h"
#include "Ability/AttributeSet_Default.h"
#include "Character/Monster/AttributeSet_Monster.h"
#include "Ability/Effect/DamageEffect.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"

UVSAbility_BodyTackle::UVSAbility_BodyTackle(const FObjectInitializer& ObjectInitializer) 
			: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;
	CooldownGameplayEffectClass = UCooldownEffect::StaticClass();

	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.EnemyBodyTackle"));
	SetCooldownTag(FGameplayTag::RequestGameplayTag("CoolTime.EnemyBodyTackle"));

	FAbilityTriggerData TackleTrigger;
	TackleTrigger.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
	TackleTrigger.TriggerTag = FGameplayTag::RequestGameplayTag("CommonEvent.Hit");
	
	AbilityTriggers.Add(TackleTrigger);
}

void UVSAbility_BodyTackle::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (TriggerEventData == nullptr)
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ETeamAttitude::Type TeamAttitude = FGenericTeamId::GetAttitude(ActorInfo->OwnerActor.Get(), TriggerEventData->Target);

	if (TeamAttitude != ETeamAttitude::Hostile || !CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* InASC = ActorInfo->AbilitySystemComponent.Get();
	const IAbilitySystemInterface* TargetASI = Cast<IAbilitySystemInterface>(TriggerEventData->Target);
	UAbilitySystemComponent* TargetASC = TargetASI->GetAbilitySystemComponent();


	bool bFound = false;
	float FinalDamage = InASC->GetGameplayAttributeValue(UAttributeSet_Monster::GetBodyTackleDamageAttribute(), bFound);
	
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, UDamageEffect::StaticClass());

	DamageSpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Default::GetDamageName(), FinalDamage);

	InASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data, TargetASC);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UVSAbility_BodyTackle::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE == nullptr) return;

	float CurrentCoolTime = 0.25f;
	if (CurrentCoolTime <= 0) return;

	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, CooldownGE->GetClass());
	SpecHandle.Data->DynamicGrantedTags.AddTag(GetCooldownTag());
	SpecHandle.Data->SetSetByCallerMagnitude(UCooldownEffect::CoolTimeSetByCallerName, CurrentCoolTime);

	FActiveGameplayEffectHandle ActiveHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
}

bool UVSAbility_BodyTackle::CanSetOrAddInBook(const UAbilityBookComponent& InBook, int NewLevel) const
{
	return false;
}
