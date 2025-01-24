// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/AttributeSet_Monster.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemComponent.h"

void UAttributeSet_Monster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Monster, BountyEXP, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Monster, BountyGold, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Monster, BodyTackleDamage, COND_None, REPNOTIFY_OnChanged);

}

void UAttributeSet_Monster::OnRep_BountyEXP(const FGameplayAttributeData& OldBountyEXP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Monster, BountyEXP, OldBountyEXP);
}

void UAttributeSet_Monster::OnRep_BountyGold(const FGameplayAttributeData& OldBountyGold)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Monster, BountyGold, OldBountyGold);
}

void UAttributeSet_Monster::OnRep_BodyTackleDamage(const FGameplayAttributeData& OldBodyTackleDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Monster, BodyTackleDamage, OldBodyTackleDamage);

}
