// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/VSMonster.h"
#include "Character/Monster/MonsterAIController.h"
#include "AbilitySystemComponent.h"
#include "Ability/AttributeSet_Default.h"
#include "Character/Monster/AttributeSet_Monster.h"
#include "Character/Monster/InitAttriuteEffect_Monster.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffectTypes.h"
#include "ProjectVS.h"
#include "Components/CapsuleComponent.h"
#include "Character/Monster/MonsterWorldManager.h"
#include "PlayGameState.h"

// Sets default values
AVSMonster::AVSMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = false;

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystem");
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	ATS_Default = CreateDefaultSubobject<UAttributeSet_Default>("AttributeSet_Default");
	AbilitySystem->AddSpawnedAttribute(ATS_Default);

	ATS_Monster = CreateDefaultSubobject<UAttributeSet_Monster>("AttributeSet_Monster");
	AbilitySystem->AddSpawnedAttribute(ATS_Monster);

	
}

// Called when the game starts or when spawned
void AVSMonster::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeAbilitySystem();
}

void AVSMonster::BeginDestroy()
{
	Super::BeginDestroy();
}

// Called every frame
void AVSMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVSMonster::InitializeAbilitySystem()
{
	if(GetLocalRole() != ENetRole::ROLE_Authority) return;

	AbilitySystem->InitAbilityActorInfo(this, this);
	
	AbilitySystem->GetGameplayAttributeValueChangeDelegate(UAttributeSet_Default::GetHPAttribute()).AddUObject(this, &AVSMonster::OnHPChanged);
		
	AbilitySystem->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag("State.Dead")).AddUObject(this, &AVSMonster::OnDeadTagAdded);

	RegisterAbilitySystem(AbilitySystem);
}

void AVSMonster::OnHPChanged(const FOnAttributeChangeData& InData)
{
	LOG_ERROR(TEXT("%s HP Change to {%.1f}"), *GetName(), InData.NewValue);

	if (InData.NewValue <= 0)
	{
		AbilitySystem->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag("State.Dead"));
	}
}


void AVSMonster::OnDeadTagAdded(FGameplayTag InTag, int NewCount)
{
	GetWorld()->GetGameState<APlayGameState>();

	OnMonsterDead.Broadcast(this);


	Destroy();
}

UAbilitySystemComponent* AVSMonster::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

FGenericTeamId AVSMonster::GetGenericTeamId() const
{
	if (AMonsterAIController* MonsterAIController = GetController<AMonsterAIController>())
	{
		return MonsterAIController->GetGenericTeamId();
	}

	return FGenericTeamId::NoTeam;
}

float AVSMonster::GetBountyEXP() const
{
	return ATS_Monster->GetBountyEXP();
}

float AVSMonster::GetBountyGold() const
{
	return ATS_Monster->GetBountyGold();;
}

