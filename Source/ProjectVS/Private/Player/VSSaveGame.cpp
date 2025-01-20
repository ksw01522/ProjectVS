// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/VSSaveGame.h"

UVSSaveGame::UVSSaveGame() : Gold(5000)
{
	
}

void UVSSaveGame::SetGold(int NewGold)
{
	if(NewGold == Gold) return;

	int PrevGold = Gold;

	Gold = NewGold;
	
	int CurrentGold = Gold;

	if (OnGoldChangedDelegate.IsBound())
	{
		OnGoldChangedDelegate.Broadcast(PrevGold, CurrentGold);
	}
}

void UVSSaveGame::GainGold(int InGold)
{
	SetGold(Gold + InGold);
}

void UVSSaveGame::LoseGold(int InGold)
{
	SetGold(Gold - InGold);
}

void UVSSaveGame::UpgradePlayer(const FName& Target, int PayGold, int InLevel)
{
	UpgradeInfo.FindOrAdd(Target) = InLevel;
	LoseGold(PayGold);
}

int UVSSaveGame::GetUpgradeLevel(const FName& Target) const
{
	if (const int* TargetPtr = UpgradeInfo.Find(Target))
	{
		return *TargetPtr;
	}
	else
	{
		return 0;
	}
}





