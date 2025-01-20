// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Ability/AttributeSetHelper.h"
#include "AttributeSet_Player.generated.h"

#define ATTRIBUTE_CATEGORY "Player"


/**
 * 
 */



UCLASS()
class PROJECTVS_API UAttributeSet_Player : public UAttributeSet
{
	GENERATED_BODY()
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;


private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HPRegen, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData HPRegen;

	UFUNCTION()
	void OnRep_HPRegen(const FGameplayAttributeData& OldHPRegen);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, HPRegen);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_DamageRate, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData DamageRate;

	UFUNCTION()
	void OnRep_DamageRate(const FGameplayAttributeData& OldDamageRate);

public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, DamageRate);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CoolTimeRate, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData CoolTimeRate;

	UFUNCTION()
	void OnRep_CoolTimeRate(const FGameplayAttributeData& OldCoolTimeReduction);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, CoolTimeRate);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorP, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData ArmorP;

	UFUNCTION()
	void OnRep_ArmorP(const FGameplayAttributeData& OldArmorP);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, ArmorP);


private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ProjectileSpeed, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData ProjectileSpeed;

	UFUNCTION()
	void OnRep_ProjectileSpeed(const FGameplayAttributeData& OldProjectileSpeed);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, ProjectileSpeed);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ProjectileCount, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData ProjectileCount;

	UFUNCTION()
	void OnRep_ProjectileCount(const FGameplayAttributeData& OldProjectileCount);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, ProjectileCount);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EffectTimeRate, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData EffectTimeRate;

	UFUNCTION()
	void OnRep_EffectTimeRate(const FGameplayAttributeData& OldEffectTimeRate);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, EffectTimeRate);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EffectRangeRate, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData EffectRangeRate;

	UFUNCTION()
	void OnRep_EffectRangeRate(const FGameplayAttributeData& OldEffectRangeRate);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, EffectRangeRate);


private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagnetSize, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MagnetSize;

	UFUNCTION()
	void OnRep_MagnetSize(const FGameplayAttributeData& OldMagnetSize);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, MagnetSize);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Lucky, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Lucky;

	UFUNCTION()
	void OnRep_Lucky(const FGameplayAttributeData& OldLucky);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, Lucky);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Curse, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Curse;

	UFUNCTION()
	void OnRep_Curse(const FGameplayAttributeData& OldCurse);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, Curse);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResurrectionCount, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData ResurrectionCount;

	UFUNCTION()
	void OnRep_ResurrectionCount(const FGameplayAttributeData& OldResurrectionCount);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, ResurrectionCount);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Greed, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Greed;

	UFUNCTION()
	void OnRep_Greed(const FGameplayAttributeData& OldGreed);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, Greed);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EXPRate, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData EXPRate;

	UFUNCTION()
	void OnRep_EXPRate(const FGameplayAttributeData& OldEXPRate);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, EXPRate);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_RefreshCount, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData RefreshCount;

	UFUNCTION()
	void OnRep_RefreshCount(const FGameplayAttributeData& OldRefreshCount);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, RefreshCount);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CurrentEXP, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData CurrentEXP;

	UFUNCTION()
	void OnRep_CurrentEXP(const FGameplayAttributeData& OldCurrentEXP);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EXPForLevelUp, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData EXPForLevelUp;

	UFUNCTION()
	void OnRep_EXPForLevelUp(const FGameplayAttributeData& OldEXPForLevelUp);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EXPForLevelUp_Base, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData EXPForLevelUp_Base;

	UFUNCTION()
	void OnRep_EXPForLevelUp_Base(const FGameplayAttributeData& OldEXPForLevelUp_Base);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_EXPForLevelUp_ByLevel, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData EXPForLevelUp_ByLevel;


	UFUNCTION()
	void OnRep_EXPForLevelUp_ByLevel(const FGameplayAttributeData& OldEXPForLevelUp_ByLevel);


public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, CurrentEXP)

	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, EXPForLevelUp)

	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, EXPForLevelUp_Base)

	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, EXPForLevelUp_ByLevel)

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Gold, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Gold;

	UFUNCTION()
	void OnRep_Gold(const FGameplayAttributeData& OldGold);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Player, Gold);
};

#undef ATTRIBUTE_CATEGORY