// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Ability/AttributeSetHelper.h"
#include "AttributeSet_Monster.generated.h"

#define ATTRIBUTE_CATEGORY "Monster"

/**
 * 
 */
UCLASS()
class PROJECTVS_API UAttributeSet_Monster : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


private:
	UPROPERTY(ReplicatedUsing = OnRep_BountyEXP)
	FGameplayAttributeData BountyEXP;

	UFUNCTION()
	void OnRep_BountyEXP(const FGameplayAttributeData& OldBountyEXP);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Monster, BountyEXP)

private:
	UPROPERTY(ReplicatedUsing = OnRep_BountyGold)
	FGameplayAttributeData BountyGold;

	UFUNCTION()
	void OnRep_BountyGold(const FGameplayAttributeData& OldBountyGold);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Monster, BountyGold)

};

#undef ATTRIBUTE_CATEGORY