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
#include "PaperFlipbookComponent.h"
#include "PlayGameState.h"
#include "Ability/Ability/Enemy/VSAbility_BodyTackle.h"

// Sets default values
AVSMonster::AVSMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = false;

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystem");
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	ATS_Default = CreateDefaultSubobject<UAttributeSet_Default>("AttributeSet_Default");
	AbilitySystem->AddSpawnedAttribute(ATS_Default);

	ATS_Monster = CreateDefaultSubobject<UAttributeSet_Monster>("AttributeSet_Monster");
	AbilitySystem->AddSpawnedAttribute(ATS_Monster);

	GetSprite()->SetNotifyRigidBodyCollision(false);
}

// Called when the game starts or when spawned
void AVSMonster::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeAbilitySystem();

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AVSMonster::OnCharaterHit);
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

	AbilitySystem->GiveAbility(FGameplayAbilitySpec(UVSAbility_BodyTackle::StaticClass(), 1, -1, this));
}

void AVSMonster::OnHPChanged(const FOnAttributeChangeData& InData)
{
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

void AVSMonster::OnCharaterHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(OtherActor == nullptr) return;
	

	FGameplayEventData EventData;
	EventData.Target = OtherActor;
	AbilitySystem->HandleGameplayEvent(FGameplayTag::RequestGameplayTag("CommonEvent.Hit"), &EventData);

	FVector NegativeDist = GetActorLocation() - OtherActor->GetActorLocation();
	NegativeDist.Z = 0;

	NegativeDist.Normalize();

	AddActorWorldOffset(NegativeDist);
}

