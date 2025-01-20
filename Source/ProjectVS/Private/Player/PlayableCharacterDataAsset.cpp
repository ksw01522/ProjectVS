// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayableCharacterDataAsset.h"

UPlayableCharacterDataAsset::UPlayableCharacterDataAsset()
	:	MaxHP(100), HPRegen(0), MoveSpeed(200), DamageRate(1), CoolTimeRate(1), ArmorP(0),
		ProjectileSpeed(0), ProjectileCount(0), EffectTimeRate(1), EffectRangeRate(1), MagnetSize(0), Lucky(0),
		Curse(0), ResurrectionCount(0), Greed(1), EXPRate(1), RefreshCount(0)
{
	
}

FPrimaryAssetId UPlayableCharacterDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(GetPlayableCharacterAssetType(), CharacterName);
}

FPrimaryAssetType UPlayableCharacterDataAsset::GetPlayableCharacterAssetType()
{
	return FPrimaryAssetType("PlayableCharacter");
}
