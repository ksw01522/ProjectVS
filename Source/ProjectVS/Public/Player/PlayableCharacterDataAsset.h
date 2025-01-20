// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayableCharacterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVS_API UPlayableCharacterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPlayableCharacterDataAsset();

protected:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	static FPrimaryAssetType GetPlayableCharacterAssetType();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	FName CharacterName;

	UPROPERTY(EditDefaultsOnly, Category = "Basic", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float StartLevel;

	UPROPERTY(EditDefaultsOnly, Category = "Basic", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float MaxHP;
	
	UPROPERTY(EditDefaultsOnly, Category = "Basic", meta = (AllowPrivateAccess = "true"))
	float HPRegen;

	UPROPERTY(EditDefaultsOnly, Category = "Basic", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float MoveSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Basic", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float DamageRate;

	UPROPERTY(EditDefaultsOnly, Category = "Basic", meta = (AllowPrivateAccess = "true"))
	float CoolTimeRate;

	UPROPERTY(EditDefaultsOnly, Category = "Basic", meta = (AllowPrivateAccess = "true"))
	float ArmorP;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	float ProjectileSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	float ProjectileCount;

	UPROPERTY(EditDefaultsOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	float EffectTimeRate;

	UPROPERTY(EditDefaultsOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	float EffectRangeRate;

	UPROPERTY(EditDefaultsOnly, Category = "Bonus", meta = (AllowPrivateAccess = "true"))
	float MagnetSize;

	UPROPERTY(EditDefaultsOnly, Category = "Bonus", meta = (AllowPrivateAccess = "true"))
	float Lucky;
	
	UPROPERTY(EditDefaultsOnly, Category = "Bonus", meta = (AllowPrivateAccess = "true"))
	float Curse;

	UPROPERTY(EditDefaultsOnly, Category = "Bonus", meta = (AllowPrivateAccess = "true"))
	float Greed;

	UPROPERTY(EditDefaultsOnly, Category = "Bonus", meta = (AllowPrivateAccess = "true"))
	float EXPRate;

	UPROPERTY(EditDefaultsOnly, Category = "Bonus", meta = (AllowPrivateAccess = "true"))
	float RefreshCount;

	UPROPERTY(EditDefaultsOnly, Category = "Bonus", meta = (AllowPrivateAccess = "true"))
	float ResurrectionCount;

	UPROPERTY(EditDefaultsOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<class UVSAbility>> Abilities;

	UPROPERTY(EditDefaultsOnly, Category = "EXP", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float EXPForLevelUp_Base;

	UPROPERTY(EditDefaultsOnly, Category = "EXP", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	float EXPForLevelUp_ByLevel;

public:
	FName GetCharacterName() const { return CharacterName; }

	float GetStartLevel() const { return StartLevel; }

	float GetMaxHP() const { return MaxHP;}

	float GetMoveSpeed() const { return MoveSpeed;}
	
	float GetHPRegen() const { return HPRegen;}

	float GetDamageRate() const { return DamageRate;}
	
	float GetCoolTimeRate() const { return CoolTimeRate;}
	
	float GetArmorP() const { return ArmorP;}
	
	float GetProjectileSpeed() const { return ProjectileSpeed;}
	
	float GetProjectileCount() const { return ProjectileCount;}
	
	float GetEffectTimeRate() const { return EffectTimeRate;}
	
	float GetEffectRangeRate() const { return EffectRangeRate;}
	
	float GetMagnetSize() const { return MagnetSize;}

	float GetLucky() const { return Lucky;}

	float GetCurse() const { return Curse;}

	float GetResurrectionCount() const { return ResurrectionCount; }
	
	float GetGreed() const { return Greed; }
	
	float GetEXPRate() const { return EXPRate; }
	
	float GetRefreshCount() const { return RefreshCount; }

	TArray<TSubclassOf<UVSAbility>> GetAbilities() const { return Abilities; }

	float GetEXPForLevelUp_Base() const {return EXPForLevelUp_Base; }

	float GetEXPForLevelUp_ByLevel() const { return EXPForLevelUp_ByLevel; }
};


