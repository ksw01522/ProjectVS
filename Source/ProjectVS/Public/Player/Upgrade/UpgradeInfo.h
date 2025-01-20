// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UpgradeInfo.generated.h"

/**
 * 
 */
USTRUCT()
struct FUpgradeInfo
{
	GENERATED_BODY()
public:
	FUpgradeInfo() : MaxHPLevel(0)
	{}
	~FUpgradeInfo()
	{}

private:
	UPROPERTY()
	int MaxHPLevel;

public:
	int GetMaxHPLevel() const { return MaxHPLevel; }
	void SetMaxHPLevel(int NewLevel) { MaxHPLevel = NewLevel;}

};
