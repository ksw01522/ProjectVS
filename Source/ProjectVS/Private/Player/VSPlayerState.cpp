// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/VSPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Ability/AttributeSet_Default.h"
#include "Player/Ability/AttributeSet_Player.h"
#include "Ability/AbilityBookComponent.h"
#include "Ability/Effect/CooldownEffect.h"
#include "Net/UnrealNetwork.h"

#include "Character/CharacterDataManager.h"

#include "Player/PlayableCharacterDataAsset.h"
#include "Player/VSLocalPlayer.h"
#include "Player/InitAttributeEffect_Player.h"

#include "Player/Upgrade/PlayerUpgradeSubsystem.h"

#include "Ability/VSAbility.h"

#include "ProjectVS.h"

#include "VSGameInstance.h"


AVSPlayerState::AVSPlayerState()
{	
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystem");
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ATS_Default = CreateDefaultSubobject<UAttributeSet_Default>("AttributeSet_Default");
	ATS_Player = CreateDefaultSubobject<UAttributeSet_Player>("AttributeSet_Player");

	ASC->AddSpawnedAttribute(ATS_Default);
	ASC->AddSpawnedAttribute(ATS_Player);

	AbilityBook = CreateDefaultSubobject<UAbilityBookComponent>("AbilityBook");
}

void AVSPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = GetPlayerController())
	{
		if (UVSLocalPlayer* LocalPlayer = Cast<UVSLocalPlayer>(PlayerController->GetLocalPlayer()))
		{
			SetCharacterName(LocalPlayer->GetSelectedCharacterName());
		}
	}

	AbilityBook->RegisterAbilitySystem(ASC);
}

void AVSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(AVSPlayerState, CharacterName, COND_None, REPNOTIFY_OnChanged);
}

void AVSPlayerState::SetCharacterName(const FName& NewName)
{
	CharacterName = NewName;

	if (CharacterName.IsNone())
	{
		CharacterName = "Default";
	}

	AbilityBook->InitializeAddableAbilities(CharacterName);
	InitializeASC();
}


#define INIT_ATTRIBUTE(A, AS) \
{\
	const UUpgradeDataAsset* A##Asset = UpgradeSS->FindUpgradeAsset(#A); \
	float Value = CharacterData->Get##A() + A##Asset->GetPower(VSGameInst->GetUpgradeLevel(#A)); \
	EffectSpec->SetSetByCallerMagnitude(AS::Get##A##Name(), Value); \
}


void AVSPlayerState::InitializeASC()
{
	UVSGameInstance* VSGameInst = GetGameInstance<UVSGameInstance>();
	UPlayerUpgradeSubsystem* UpgradeSS = UPlayerUpgradeSubsystem::GetPlayerUpgradeSubsystem();

	const UCharacterDataManager* CharacterDataManager = UCharacterDataManager::GetCharacterDataManager();

	const UPlayableCharacterDataAsset* CharacterData = CharacterDataManager->FindPlayableCharacterData(CharacterName);
	
	//Initialize Character Attribute;
	if (CharacterData)
	{
		FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(UInitAttributeEffect_Player::StaticClass(), 0, ASC->MakeEffectContext());
		FGameplayEffectSpec* EffectSpec = SpecHandle.Data.Get();

		EffectSpec->SetSetByCallerMagnitude(UAttributeSet_Default::GetLevelName(), 1);

		INIT_ATTRIBUTE(MaxHP, UAttributeSet_Default)

		INIT_ATTRIBUTE(MoveSpeed, UAttributeSet_Default)

		INIT_ATTRIBUTE(HPRegen, UAttributeSet_Player)

		INIT_ATTRIBUTE(DamageRate, UAttributeSet_Player)
		
		INIT_ATTRIBUTE(CoolTimeRate, UAttributeSet_Player)
		
		INIT_ATTRIBUTE(ArmorP, UAttributeSet_Player)
		
		INIT_ATTRIBUTE(ProjectileSpeed, UAttributeSet_Player)

		INIT_ATTRIBUTE(ProjectileCount, UAttributeSet_Player)

		INIT_ATTRIBUTE(EffectTimeRate, UAttributeSet_Player);
		
		INIT_ATTRIBUTE(EffectRangeRate, UAttributeSet_Player);
		
		INIT_ATTRIBUTE(MagnetSize, UAttributeSet_Player);
		
		INIT_ATTRIBUTE(Lucky, UAttributeSet_Player);
		
		INIT_ATTRIBUTE(Curse, UAttributeSet_Player);
		
		INIT_ATTRIBUTE(ResurrectionCount, UAttributeSet_Player);
		
		INIT_ATTRIBUTE(Greed, UAttributeSet_Player);
		
		INIT_ATTRIBUTE(EXPRate, UAttributeSet_Player);
		
		INIT_ATTRIBUTE(RefreshCount, UAttributeSet_Player);

		EffectSpec->SetSetByCallerMagnitude(UAttributeSet_Player::GetEXPForLevelUp_BaseName(), CharacterData->GetEXPForLevelUp_Base());

		EffectSpec->SetSetByCallerMagnitude(UAttributeSet_Player::GetEXPForLevelUp_ByLevelName(), CharacterData->GetEXPForLevelUp_ByLevel());

		float NewEXPForLevelUp = CharacterData->GetEXPForLevelUp_Base() + CharacterData->GetStartLevel() * CharacterData->GetEXPForLevelUp_ByLevel();
		EffectSpec->SetSetByCallerMagnitude(UAttributeSet_Player::GetEXPForLevelUpName(), NewEXPForLevelUp);

		ASC->ApplyGameplayEffectSpecToSelf(*EffectSpec);

		TArray<TSubclassOf<UVSAbility>> Abilities = CharacterData->GetAbilities();

		for (auto& Ability : Abilities)
		{
			FGameplayAbilitySpec* AbilitySpec = new FGameplayAbilitySpec(Ability);
			ASC->GiveAbility(*AbilitySpec);

			delete AbilitySpec;
		}
	}
}



#undef INIT_ATTRIBUTE

void AVSPlayerState::CheckLevelUp()
{
	float TempEXPForLevelUp = ATS_Player->GetEXPForLevelUp();
	float TempCurrentEXP = ATS_Player->GetCurrentEXP();
	float BeforeLevel = ATS_Default->GetLevel();
	float AfterLevel = BeforeLevel;
	
	while (0 < TempEXPForLevelUp && TempEXPForLevelUp <= TempCurrentEXP)
	{
		++AfterLevel;
		TempCurrentEXP -= TempEXPForLevelUp;
		TempEXPForLevelUp = ATS_Player->GetEXPForLevelUp_Base() + AfterLevel * ATS_Player->GetEXPForLevelUp_ByLevel();
	}
	
	if (BeforeLevel != AfterLevel)
	{
		ASC->ApplyModToAttribute(UAttributeSet_Default::GetLevelAttribute(), EGameplayModOp::Additive, AfterLevel - BeforeLevel);
		ASC->ApplyModToAttribute(UAttributeSet_Player::GetCurrentEXPAttribute(), EGameplayModOp::Override, TempCurrentEXP);
		ASC->ApplyModToAttribute(UAttributeSet_Player::GetEXPForLevelUpAttribute(), EGameplayModOp::Override, TempEXPForLevelUp);

		OnLevelUpDelegate.Broadcast(BeforeLevel, AfterLevel);
	}
}

void AVSPlayerState::GainGold(float InGold)
{
	if(!ASC->IsOwnerActorAuthoritative() || InGold <= 0) {return;}

	ASC->ApplyModToAttribute(UAttributeSet_Player::GetGoldAttribute(), EGameplayModOp::Additive, InGold);
}

void AVSPlayerState::SaveGold_Implementation()
{
	UVSGameInstance* GameInst = GetGameInstance<UVSGameInstance>();

	GameInst->GainGold(ATS_Player->GetGold());


}



void AVSPlayerState::GainEXP(float InEXP)
{
	if(!ASC->IsOwnerActorAuthoritative() || InEXP <= 0) { return; }
	
	float FinalEXP = InEXP * ATS_Player->GetEXPRate();

	ASC->ApplyModToAttribute(UAttributeSet_Player::GetCurrentEXPAttribute(), EGameplayModOp::Additive, FinalEXP);
	
	FTimerManager& TimerManager = GetWorldTimerManager();
	if (!TimerManager.TimerExists(CheckLevelUpTimerHandle))
	{
		CheckLevelUpTimerHandle = TimerManager.SetTimerForNextTick(this, &AVSPlayerState::CheckLevelUp);
	}
}


