// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Topspin/VSAbility_Topspin.h"
#include "AbilityDataManager.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet_Player.h"
#include "Ability/Effect/DamageEffect.h"
#include "AttributeSet_Default.h"

#include "ProjectVS.h"
#include "TopspinActor.h"

#include "Ability/AbilityBookComponent.h"

#define LOCTEXT_NAMESPACE "VSAbility"

UVSAbility_Topspin::UVSAbility_Topspin(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetAbilityType(EVSAbilityType::Active);
}

void UVSAbility_Topspin::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	
	if (AvatarActor == nullptr || !CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}


	UAbilitySystemComponent* InASC = ActorInfo->AbilitySystemComponent.Get();

	int AbLevel = GetAbilityLevel(Handle, ActorInfo);

	int FinalTopspinCount = GetTopspinCount(AbLevel, InASC);

	UWorld* World = AvatarActor->GetWorld();

	float FinalScale = GetTopspinScale(AbLevel, InASC);

	FTransform SpawnTranform;
	SpawnTranform.SetLocation(AvatarActor->GetActorLocation());

	ATopspinActor* TopspinActor = World->SpawnActorDeferred<ATopspinActor>(TopspinActorClass, SpawnTranform);
	if (TopspinActor)
	{
		FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, false);

		TopspinActor->AttachToActor(AvatarActor, AttachRule);

		TopspinActor->SetTopspinCount(FinalTopspinCount);

		FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, UDamageEffect::StaticClass());
		DamageSpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Default::GetDamageName(), GetTopspinDamage(AbLevel, InASC));

		TopspinActor->AddEffectSpec("TopspinDamage", DamageSpecHandle);

		TopspinActor->SetTopspinScale(FinalScale);
		
		TopspinActor->FinishSpawning(SpawnTranform);
		TopspinActor->StartTopspin();
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

FText UVSAbility_Topspin::GetLeveUpDescriptionText_Implementation(const UAbilityBookComponent* InBook, int BeforeLevel, int AfterLevel) const
{
	TArray<FText> DescTexts;

	float Value_Before = 0;
	float Value_After = 0;

	UAbilitySystemComponent* ASC = InBook == nullptr ? nullptr : InBook->GetAbilitySystemComponent();

	//탑스핀 데미지
	{
		Value_Before = GetTopspinDamage(BeforeLevel, ASC);
		Value_After = GetTopspinDamage(AfterLevel, ASC);
		if (Value_Before != Value_After)
		{
			FFormatOrderedArguments Args;
			Args.Add(Value_Before);
			Args.Add(Value_After);

			DescTexts.Add(FText::Format(LOCTEXT("Topspin_LevelUp Damage", "탑스핀 데미지 {0} -> {1}"), Args));
		}
	}

	//발사 크기
	{
		Value_Before = GetTopspinScale(BeforeLevel, ASC);
		Value_After = GetTopspinScale(AfterLevel, ASC);
		if (Value_Before != Value_After)
		{
			FFormatOrderedArguments Args;
			Args.Add(Value_Before);
			Args.Add(Value_After);

			DescTexts.Add(FText::Format(LOCTEXT("Topspin_LevelUp Scale", "탑스핀 크기 {0} -> {1}"), Args));
		}
	}

	//발사 회수
	{
		Value_Before = GetTopspinCount(BeforeLevel, ASC);
		Value_After = GetTopspinCount(AfterLevel, ASC);
		if (Value_Before != Value_After)
		{
			FFormatOrderedArguments Args;
			Args.Add(Value_Before);
			Args.Add(Value_After);

			DescTexts.Add(FText::Format(LOCTEXT("Topspin_LevelUp Damage", "탑스핀 회수 {0} -> {1}"), Args));
		}
	}


	return FText::Join(FText::FromString("\n"), DescTexts);
}

float UVSAbility_Topspin::GetTopspinDamage(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	float ReturnDamage = 0;

#if WITH_EDITOR
	if (ADM == nullptr)
	{
		ReturnDamage = TopspinDamage[InLevel];
	}
#endif

	bool bResult = false;

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		ReturnDamage = ADM->FindAbilityData(TopspinDamageTag, bResult, InLevel);
#if WITH_EDITOR
	}
#endif

	if (InASC != nullptr)
	{
		float DamageRate = InASC->GetGameplayAttributeValue(UAttributeSet_Player::GetDamageRateAttribute(), bResult);
		if (bResult)
		{
			ReturnDamage *= DamageRate;
		}
	}

	return ReturnDamage;
}

float UVSAbility_Topspin::GetTopspinScale(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	float RangeRate = 1;
	bool bResult = false;


#if WITH_EDITOR
	if (ADM == nullptr)
	{
		if (TopspinScale.IsValidIndex(InLevel)) { RangeRate = TopspinScale[InLevel]; }
	}
#endif

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		RangeRate = ADM->FindAbilityData(TopspinScaleTag, bResult, InLevel);
		if (!bResult) RangeRate = 1;
#if WITH_EDITOR
	}
#endif

	if (InASC != nullptr)
	{
		float ABSRangeRate = InASC->GetGameplayAttributeValue(UAttributeSet_Player::GetEffectRangeRateAttribute(), bResult);
		if (bResult) { RangeRate *= ABSRangeRate; }
	}

	return RangeRate;
}

int UVSAbility_Topspin::GetTopspinCount(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	int ReturnTopspinCount = 1;
	bool bResult = false;

#if WITH_EDITOR
	if (ADM == nullptr)
	{
		if (TopspinCount.IsValidIndex(InLevel)) { ReturnTopspinCount = TopspinCount[InLevel]; }
	} 
	else{
#endif
		ReturnTopspinCount = ADM->FindAbilityData(TopspinCountTag, bResult, InLevel);
		if (!bResult) { ReturnTopspinCount = 1; }
#if WITH_EDITOR
	}
#endif

	if (InASC != nullptr)
	{
		int ASCCount = InASC->GetGameplayAttributeValue(UAttributeSet_Player::GetProjectileCountAttribute(), bResult);
		if (bResult) { ReturnTopspinCount += ASCCount; }
	}

	return ReturnTopspinCount;
}

#if WITH_EDITOR
void UVSAbility_Topspin::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PName = PropertyChangedEvent.GetMemberPropertyName();
	if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_Topspin, TopspinDamageTag))
	{
		ApplyDataFromDataManager(TopspinDamageTag, TopspinDamage);
	}
	else if(PName == GET_MEMBER_NAME_CHECKED(UVSAbility_Topspin, TopspinScaleTag))
	{
		ApplyDataFromDataManager(TopspinScaleTag, TopspinScale);
	}
	else if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_Topspin, TopspinCountTag))
	{
		ApplyDataFromDataManager(TopspinCountTag, TopspinCount);
	}
}

void UVSAbility_Topspin::CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const
{
	Super::CreateAbilityDataToSave(InMap);

	if (TopspinDamageTag.IsValid()) { InMap.Add(TopspinDamageTag.GetTagName(), TopspinDamage); }
	if (TopspinScaleTag.IsValid()) { InMap.Add(TopspinScaleTag.GetTagName(), TopspinScale); }
	if (TopspinCountTag.IsValid()) { InMap.Add(TopspinCountTag.GetTagName(), TopspinCount); }
}

void UVSAbility_Topspin::NativeLoadDataFromDataManager()
{
	Super::NativeLoadDataFromDataManager();

	ApplyDataFromDataManager(TopspinDamageTag, TopspinDamage);
	ApplyDataFromDataManager(TopspinScaleTag, TopspinScale);
	ApplyDataFromDataManager(TopspinCountTag, TopspinCount);
}
#endif

#undef LOCTEXT_NAMESPACE