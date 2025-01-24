// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/MonsterWorldManager.h"
#include "Character/CharacterDataManager.h"
#include "Character/Monster/VSMonster.h"
#include "Character/Monster/InitAttriuteEffect_Monster.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "Ability/AttributeSet_Default.h"
#include "Character/Monster/InitAttriuteEffect_Monster.h"
#include "Character/Monster/AttributeSet_Monster.h"
#include "PlayGameState.h"
#include "ProjectVSGameMode.h"
#include "ProjectVS.h"
#include "Player/VSPlayerState.h"

void UMonsterWorldManager::OnMonsterDead(AVSMonster* Target)
{
	if(Target == nullptr) return;

	if (APlayGameState* GameState = GetWorld()->GetGameState<APlayGameState>())
	{
		GameState->SetMonsteKillCount(GameState->GetMonsterKillCount() + 1);
	}

	if (OnMonsterDeadDel.IsBound())
	{
		OnMonsterDeadDel.Broadcast(Target);
	}

	RemoveMonster(Target);
}

bool UMonsterWorldManager::ShouldCreateSubsystem(UObject* Outer) const
{
	bool bShouldCreate = Super::ShouldCreateSubsystem(Outer);
	if(!bShouldCreate) return false;

	UWorld* World = Cast<UWorld>(Outer);
	ENetMode NetMode = World->GetNetMode();

	return NetMode == ENetMode::NM_Standalone || NetMode == ENetMode::NM_ListenServer || NetMode == ENetMode::NM_DedicatedServer;
}

void UMonsterWorldManager::OnWorldBeginPlay(UWorld& InWorld)
{
	
	
}

TArray<AVSMonster*> UMonsterWorldManager::SpawnMonseters(const FName& InCode, float Level, const TArray<FVector>& SpawnLocations)
{
	TArray<AVSMonster*> SpawnedMonsters;
	
	if (SpawnLocations.IsEmpty())
	{
		return SpawnedMonsters;
	}

	ENetMode NetMode = GetWorld()->GetNetMode();
	if(NetMode == ENetMode::NM_Client) return SpawnedMonsters;

	const UCharacterDataManager* CharacterDataManager = UCharacterDataManager::GetCharacterDataManager();
	
	const FMonsterDataStruct* FindedData = CharacterDataManager->FindMonsterData(InCode);

	TSubclassOf<AVSMonster> MonsterClass = CharacterDataManager->GetMonsterClass(InCode);

	float MonsterLevel = Level;

	float MaxHP = FindedData->HP_Base + FindedData->HP_Level * Level;

	float ATKP = FindedData->ATK_Base + FindedData->ATK_Level * Level;

	float MoveSpeed = FindedData->MoveSpeed;

	float BountyEXP = FindedData->BountyEXP;
	float BountyGold = FindedData->BountyGold;
	float BodyTackleDamage = FindedData->BodyTackleDamage;

	for (const auto& SpawnLocation : SpawnLocations)
	{
		FActorSpawnParameters SpawnParamaters;
		SpawnParamaters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AVSMonster* SpawnedMonster = GetWorld()->SpawnActor<AVSMonster>(MonsterClass.Get(), SpawnLocation, FRotator::ZeroRotator, SpawnParamaters);

		if (SpawnedMonster)
		{
			SpawnedMonsters.Add(SpawnedMonster);
			WeakMonsters.Add(SpawnedMonster);

			SpawnedMonster->GetOnMonsterDeadDelegate().AddUObject(this, &UMonsterWorldManager::OnMonsterDead);

			UAbilitySystemComponent* ASC = SpawnedMonster->GetAbilitySystemComponent();

			FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(UInitAttriuteEffect_Monster::StaticClass(), 1, ASC->MakeEffectContext());

			SpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Default::GetLevelName(), MonsterLevel);
			SpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Default::GetMaxHPName(), MaxHP);
			SpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Default::GetMoveSpeedName(), MoveSpeed);
			SpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Monster::GetBountyEXPName(), BountyEXP);
			SpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Monster::GetBountyGoldName(), BountyGold);
			SpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Monster::GetBodyTackleDamageName(), BodyTackleDamage);

			ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
		}
	}

	return SpawnedMonsters;
}

void UMonsterWorldManager::GetMonsters(TArray<AVSMonster*>& Out) const
{
	Out.Empty();
	Out.Reserve(GetMonsterNum());

	for (auto& WeakMonster : WeakMonsters)
	{
		if(WeakMonster.IsValid()) Out.Add(WeakMonster.Get());
	}
}

void UMonsterWorldManager::RemoveMonster(AVSMonster* Target)
{
	WeakMonsters.Remove(Target);
}
