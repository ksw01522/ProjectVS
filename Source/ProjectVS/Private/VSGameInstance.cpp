// Fill out your copyright notice in the Description page of Project Settings.


#include "VSGameInstance.h"
#include "Player/VSSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Upgrade/PlayerUpgradeSubsystem.h"
#include "ProjectVS.h"


UVSGameInstance::UVSGameInstance()
{
	Gold = 0;
	CurrentSave = nullptr;
}

void UVSGameInstance::Init()
{
	Super::Init();

	LoadVSGame();
}

void UVSGameInstance::SaveVSGame()
{
	check(CurrentSave)
	UGameplayStatics::SaveGameToSlot(CurrentSave, "ProjectVS", 0);
}

void UVSGameInstance::LoadVSGame()
{
	if (!UGameplayStatics::DoesSaveGameExist("ProjectVS", 0))
	{
		CurrentSave = Cast<UVSSaveGame>(UGameplayStatics::CreateSaveGameObject(UVSSaveGame::StaticClass()));


		UGameplayStatics::SaveGameToSlot(CurrentSave, "ProjectVS", 0);

		return;
	}

	CurrentSave = Cast<UVSSaveGame>(UGameplayStatics::LoadGameFromSlot("ProjectVS", 0));

#if WITH_EDITOR
	if (CurrentSave->GetGold() < 5000 && false)
	{
		CurrentSave->SetGold(5000);
	}
#endif
}

int UVSGameInstance::GetGold() const
{
	return CurrentSave->GetGold();
}

void UVSGameInstance::GainGold(int InGold)
{
	CurrentSave->GainGold(InGold);
}

void UVSGameInstance::LoseGold(int InGold)
{
	CurrentSave->LoseGold(InGold);

}

FOnGoldChanged& UVSGameInstance::GetOnGoldChangedDelegate() const
{
	return CurrentSave->GetOnGoldChangedDelegate();
}

bool UVSGameInstance::CanPayGold(int TargetGold)
{
	return TargetGold <= GetGold();
}

void UVSGameInstance::UpgradePlayer(const FName& Target, int PayGold, int InLevel)
{
	CurrentSave->UpgradePlayer(Target, PayGold, InLevel);

	SaveVSGame();
}

int UVSGameInstance::GetUpgradeLevel(const FName& Target) const
{
	return CurrentSave->GetUpgradeLevel(Target);
}

void UVSGameInstance::LoadPlayMap()
{
	APlayerController* LocalP = GetFirstLocalPlayerController();
	if(LocalP == nullptr) return;

	UGameplayStatics::OpenLevelBySoftObjectPtr(LocalP, PlayLevel);

}

void UVSGameInstance::LoadTitleMap()
{
	APlayerController* LocalP = GetFirstLocalPlayerController();
	if (LocalP == nullptr) return;

	UGameplayStatics::OpenLevelBySoftObjectPtr(LocalP, TitleLevel);

}

