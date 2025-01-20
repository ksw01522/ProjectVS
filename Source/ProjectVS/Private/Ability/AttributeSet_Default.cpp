// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AttributeSet_Default.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "ProjectVS.h"



void UAttributeSet_Default::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Default, Level, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Default, HP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Default, MaxHP, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Default, MoveSpeed, COND_None, REPNOTIFY_Always);
}

bool UAttributeSet_Default::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
    return true;
}

void UAttributeSet_Default::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    if (Data.EvaluatedData.Attribute == GetDamageAttribute())
    {
        float LocalDamage = GetDamage();
        InitDamage(0);

        SetHP(GetHP() - LocalDamage);
    }
}

void UAttributeSet_Default::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
    if (Attribute == GetHPAttribute())
    {
        if (GetMaxHP() < NewValue)
        {
            MaxOverHP(GetMaxHP() - NewValue);
            NewValue = GetMaxHP();
        }
        else
        {
            NewValue = FMath::Max<float>(NewValue, 0);
        }
    }
}

void UAttributeSet_Default::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
    if (Attribute == GetMaxHPAttribute())
    {
        AdjustAttributeForMaxChange(HP, MaxHP, OldValue, NewValue, GetHPAttribute());
    }
}

void UAttributeSet_Default::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
    if (Attribute == GetMaxHPAttribute())
    {
        NewValue = FMath::Max<float>(0, NewValue);
    }
}

void UAttributeSet_Default::OnRep_HP(const FGameplayAttributeData& OldHP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Default, HP, OldHP)
}

void UAttributeSet_Default::OnRep_MaxHP(const FGameplayAttributeData& OldMaxHP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Default, MaxHP, OldMaxHP)

}

void UAttributeSet_Default::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Default, MoveSpeed, OldMoveSpeed);
}

void UAttributeSet_Default::MaxOverHP(float Delta)
{
    if(Delta <= 0) return;
}

void UAttributeSet_Default::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float OldMaxValue, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
    UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();

    if (!FMath::IsNearlyEqual(OldMaxValue, NewMaxValue) && AbilityComp)
    {
        float CurrentValue = AffectedAttribute.GetCurrentValue();
        float NewDelta = (OldMaxValue > 0.f) ? (CurrentValue * NewMaxValue / OldMaxValue) - CurrentValue : NewMaxValue;

        AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
    }

}

void UAttributeSet_Default::OnRep_Level(const FGameplayAttributeData& OldLevel)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Default, Level, OldLevel);
}
