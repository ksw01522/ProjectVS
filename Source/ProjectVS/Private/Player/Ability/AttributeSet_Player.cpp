// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Ability/AttributeSet_Player.h"
#include "Net/UnrealNetwork.h"
#include "ProjectVS.h"
#include "GameplayEffectExtension.h"
#include "VSGameInstance.h"

#if WITH_EDITOR
	#define MUST_DEBUG false
#endif WITH_EDITOR
void UAttributeSet_Player::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, HPRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, DamageRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, CoolTimeRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, ArmorP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, ProjectileSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, ProjectileCount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, EffectTimeRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, EffectRangeRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, MagnetSize, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, Lucky, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, Curse, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, ResurrectionCount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, Greed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, EXPRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, RefreshCount, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, CurrentEXP, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, EXPForLevelUp, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, Gold, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, EXPForLevelUp_Base, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSet_Player, EXPForLevelUp_ByLevel, COND_None, REPNOTIFY_OnChanged);
}

bool UAttributeSet_Player::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetCurrentEXPAttribute() && Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive)
	{
		float FinalEXP = Data.EvaluatedData.Magnitude * GetEXPRate();

#if WITH_EDITOR
		if (MUST_DEBUG)
		{
			LOG_WARNING(TEXT("Gain EXP : Base = %.1f, Rate = %.2f, Final = %.1f"), Data.EvaluatedData.Magnitude, GetEXPRate(), FinalEXP);
		}
#endif

		Data.EvaluatedData.Magnitude = FinalEXP;
		return true;
	}


	if (Data.EvaluatedData.Attribute == GetGoldAttribute() && Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive)
	{
		float FinalGold = Data.EvaluatedData.Magnitude * GetGreed();

		Data.EvaluatedData.Magnitude = FinalGold;
		return true;
	}

	return true;
}

void UAttributeSet_Player::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
}

void UAttributeSet_Player::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	
}

void UAttributeSet_Player::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
#if WITH_EDITOR
	if (MUST_DEBUG)
	{
		LOG_WARNING(TEXT("%s Attribute : %.1f -> %.1f"), *Attribute.AttributeName, OldValue, NewValue);
	}
#endif

}

void UAttributeSet_Player::OnRep_HPRegen(const FGameplayAttributeData& OldHPRegen)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, HPRegen, OldHPRegen);
}

void UAttributeSet_Player::OnRep_DamageRate(const FGameplayAttributeData& OldDamageRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, DamageRate, OldDamageRate);
}

void UAttributeSet_Player::OnRep_CoolTimeRate(const FGameplayAttributeData& OldCoolTimeReduction)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, CoolTimeRate, OldCoolTimeReduction);
}

void UAttributeSet_Player::OnRep_ArmorP(const FGameplayAttributeData& OldArmorP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, ArmorP, OldArmorP);
}

void UAttributeSet_Player::OnRep_ProjectileSpeed(const FGameplayAttributeData& OldProjectileSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, ProjectileSpeed, OldProjectileSpeed);
}

void UAttributeSet_Player::OnRep_ProjectileCount(const FGameplayAttributeData& OldProjectileCount)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, ProjectileCount, OldProjectileCount);
}

void UAttributeSet_Player::OnRep_EffectTimeRate(const FGameplayAttributeData& OldEffectTimeRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, EffectTimeRate, OldEffectTimeRate);
}

void UAttributeSet_Player::OnRep_EffectRangeRate(const FGameplayAttributeData& OldEffectRangeRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, EffectRangeRate, OldEffectRangeRate);
}

void UAttributeSet_Player::OnRep_MagnetSize(const FGameplayAttributeData& OldMagnetSize)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, MagnetSize, OldMagnetSize);
}

void UAttributeSet_Player::OnRep_Lucky(const FGameplayAttributeData& OldLucky)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, Lucky, OldLucky);
}

void UAttributeSet_Player::OnRep_Curse(const FGameplayAttributeData& OldCurse)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, Curse, OldCurse);
}

void UAttributeSet_Player::OnRep_ResurrectionCount(const FGameplayAttributeData& OldResurrectionCount)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, ResurrectionCount, OldResurrectionCount);
}

void UAttributeSet_Player::OnRep_Greed(const FGameplayAttributeData& OldGreed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, Greed, OldGreed);
}

void UAttributeSet_Player::OnRep_EXPRate(const FGameplayAttributeData& OldEXPRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, EXPRate, OldEXPRate);
}

void UAttributeSet_Player::OnRep_RefreshCount(const FGameplayAttributeData& OldRefreshCount)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, RefreshCount, OldRefreshCount);
}

void UAttributeSet_Player::OnRep_CurrentEXP(const FGameplayAttributeData& OldCurrentEXP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, CurrentEXP, OldCurrentEXP);

}

void UAttributeSet_Player::OnRep_EXPForLevelUp(const FGameplayAttributeData& OldEXPForLevelUp)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, EXPForLevelUp, OldEXPForLevelUp);

}

void UAttributeSet_Player::OnRep_EXPForLevelUp_Base(const FGameplayAttributeData& OldEXPForLevelUp_Base)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, EXPForLevelUp_Base, OldEXPForLevelUp_Base);
}

void UAttributeSet_Player::OnRep_EXPForLevelUp_ByLevel(const FGameplayAttributeData& OldEXPForLevelUp_ByLevel)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, EXPForLevelUp_ByLevel, OldEXPForLevelUp_ByLevel);
}

void UAttributeSet_Player::OnRep_Gold(const FGameplayAttributeData& OldGold)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSet_Player, Gold, OldGold);

}


#undef MUST_DEBUG