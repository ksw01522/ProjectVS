// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/InitAttriuteEffect_Monster.h"
#include "Character/Monster/AttributeSet_Monster.h"

UInitAttriuteEffect_Monster::UInitAttriuteEffect_Monster()
{
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Monster, BountyEXP)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Monster, BountyGold)
	ADD_ATTRIBUTE_MODIFER(UAttributeSet_Monster, BodyTackleDamage)
}
