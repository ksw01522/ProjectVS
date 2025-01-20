// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InitAttributeEffect_Player.h"
#include "Player/Ability/AttributeSet_Player.h"

UInitAttributeEffect_Player::UInitAttributeEffect_Player()
{
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, HPRegen)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, DamageRate)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, CoolTimeRate)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, ArmorP)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, ProjectileSpeed)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, ProjectileCount)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, EffectTimeRate)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, EffectRangeRate)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, MagnetSize)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, Lucky)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, Curse)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, ResurrectionCount)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, Greed)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, EXPRate)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, RefreshCount)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, EXPForLevelUp)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, EXPForLevelUp_Base)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Player, EXPForLevelUp_ByLevel)
}
