// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Gundance/VSAbility_Gundance.h"
#include "Ability/AttributeSet_Default.h"
#include "Ability/AttributeSet_Player.h"
#include "Abilities/Tasks/AbilityTask_Repeat.h"

#include "AbilityDataManager.h"
#include "AbilitySystemComponent.h"
#include "Ability/Effect/DamageEffect.h"

#include "Ability/Ability/Gundance/GundanceActor.h"
#include "Ability/AbilityBookComponent.h"

#include "Ability/AbilityDataManager.h"

#define LOCTEXT_NAMESPACE "VSAbility"

UVSAbility_Gundance::UVSAbility_Gundance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	SetAbilityType(EVSAbilityType::Active);
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UVSAbility_Gundance::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AActor* AvatarActor = ActorInfo->AvatarActor.Get();

	if (AvatarActor == nullptr || !CheckCooldown(Handle, ActorInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	UAbilitySystemComponent* CurASC = ActorInfo->AbilitySystemComponent.Get();
	int AbLevel = GetAbilityLevel();

	float FinalBladeDamage = GetBladeDamage(AbLevel, CurASC);
	CachedBladeDamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(UDamageEffect::StaticClass());
	CachedBladeDamageEffectSpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Default::GetDamageName(), FinalBladeDamage);

	float FinalShotDamage = GetShotDamage(AbLevel, CurASC);
	CachedShotDamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(UDamageEffect::StaticClass());
	CachedShotDamageEffectSpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Default::GetDamageName(), FinalShotDamage);

	float TempDanceScale = GetDanceScale(AbLevel, CurASC);

	CachedDanceScale = FVector(TempDanceScale, TempDanceScale, 1);
	float FinalDanceCount = GetDanceCount(AbLevel, CurASC);
	float FinalTimeBetweenDance = GetTimeBetweenDance(AbLevel, CurASC);

	UAbilityTask_Repeat* RepeatTask = UAbilityTask_Repeat::RepeatAction(this, 0.5, 10);

	RepeatTask->OnPerformAction.AddDynamic(this, &UVSAbility_Gundance::ShotAndBlade);

	RepeatTask->OnFinished.AddDynamic(this, &UVSAbility_Gundance::OnFinishedGundance);

	RepeatTask->ReadyForActivation();

}

void UVSAbility_Gundance::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	CachedBladeDamageEffectSpecHandle.Clear();

	CachedShotDamageEffectSpecHandle.Clear();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UVSAbility_Gundance::PreAddInBook_Implementation(UAbilityBookComponent* InBook) const
{
	InBook->SetOrAddPage("Topspin", 0);
	InBook->SetOrAddPage("HandCannon", 0);
}

bool UVSAbility_Gundance::CanSetOrAddInBook_Implementation(const UAbilityBookComponent* InBook, int NewLevel) const
{
	if(NewLevel <= 0) return true;
	if(GetMaxLevel() < NewLevel) return false;

	if (InBook->GetAbilityLevel(GetAbilityCode()) == 0)
	{
		int TopspinLevel = InBook->GetAbilityLevel("Topspin");
		int HandCannonFireLevel = InBook->GetAbilityLevel("HandCannon");

		UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

		int TopspinMaxLevel = ADM->FindAbility("Topspin").GetDefaultObject()->GetMaxLevel();
		int HandCannonFireMaxLevel = ADM->FindAbility("HandCannon").GetDefaultObject()->GetMaxLevel();
		if(TopspinLevel < TopspinMaxLevel || HandCannonFireLevel < HandCannonFireMaxLevel) { return false;}
	}

	return true;
}

FText UVSAbility_Gundance::GetLeveUpDescriptionText_Implementation(const UAbilityBookComponent* InBook, int BeforeLevel, int AfterLevel) const
{
	TArray<FText> DescTexts;

	float Value_Before = 0;
	float Value_After = 0;

	UAbilitySystemComponent* ASC = InBook == nullptr ? nullptr : InBook->GetAbilitySystemComponent();

	//칼춤 데미지
	{
		Value_Before = GetBladeDamage(BeforeLevel, ASC);
		Value_After = GetBladeDamage(AfterLevel, ASC);

		if (Value_Before != Value_After)
		{
			FFormatOrderedArguments Args;
			Args.Add(Value_Before);
			Args.Add(Value_After);

			DescTexts.Add(FText::Format(LOCTEXT("Gundance_LevelUp Blade Damage", "칼춤 데미지 {0} -> {1}"), Args));

		}
	}

	//총 데미지
	{
		Value_Before = GetShotDamage(BeforeLevel, ASC);
		Value_After = GetShotDamage(AfterLevel, ASC);

		if (Value_Before != Value_After)
		{
			FFormatOrderedArguments Args;
			Args.Add(Value_Before);
			Args.Add(Value_After);

			DescTexts.Add(FText::Format(LOCTEXT("Gundance_LevelUp Shot Damage", "총 데미지 {0} -> {1}"), Args));

		}
	}

	//춤 크기
	{
		Value_Before = GetDanceScale(BeforeLevel, ASC);
		Value_After = GetDanceScale(AfterLevel, ASC);

		if (Value_Before != Value_After)
		{
			FFormatOrderedArguments Args;
			Args.Add(Value_Before);
			Args.Add(Value_After);

			DescTexts.Add(FText::Format(LOCTEXT("Gundance_LevelUp Scale", "춤 크기 {0} -> {1}"), Args));

		}
	}

	//춤 회수
	{
		Value_Before = GetDanceCount(BeforeLevel, ASC);
		Value_After = GetDanceCount(AfterLevel, ASC);

		if (Value_Before != Value_After)
		{
			FFormatOrderedArguments Args;
			Args.Add(Value_Before);
			Args.Add(Value_After);

			DescTexts.Add(FText::Format(LOCTEXT("Gundance_LevelUp Count", "춤 횟수 {0} -> {1}"), Args));

		}
	}

	//춤 간격
	{
		Value_Before = GetTimeBetweenDance(BeforeLevel, ASC);
		Value_After = GetTimeBetweenDance(AfterLevel, ASC);

		if (Value_Before != Value_After)
		{
			FFormatOrderedArguments Args;
			Args.Add(Value_Before);
			Args.Add(Value_After);

			DescTexts.Add(FText::Format(LOCTEXT("Gundance_LevelUp TimeBetweenDance", "춤 간격 {0} -> {1}"), Args));

		}
	}

	return FText::Join(FText::FromString("\n"), DescTexts);
}

#if WITH_EDITOR

void UVSAbility_Gundance::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PName = PropertyChangedEvent.GetMemberPropertyName();
	if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_Gundance, BladeDamageTag))
	{
		ApplyDataFromDataManager(BladeDamageTag, BladeDamage);
	} 
	else if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_Gundance, ShotDamageTag))
	{
		ApplyDataFromDataManager(ShotDamageTag, ShotDamage);
	}
	else if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_Gundance, DanceScaleTag))
	{
		ApplyDataFromDataManager(DanceScaleTag, DanceScale);
	}
	else if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_Gundance, DanceCountTag))
	{
		ApplyDataFromDataManager(DanceCountTag, DanceCount);
	}
	else if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_Gundance, TimeBetweenDanceTag))
	{
		ApplyDataFromDataManager(TimeBetweenDanceTag, TimeBetweenDance);
	}

}

void UVSAbility_Gundance::CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const
{
	Super::CreateAbilityDataToSave(InMap);

	if (BladeDamageTag.IsValid()) { InMap.Add(BladeDamageTag.GetTagName(), BladeDamage); }
	if (ShotDamageTag.IsValid()) { InMap.Add(ShotDamageTag.GetTagName(), ShotDamage); }
	if (DanceScaleTag.IsValid()) { InMap.Add(DanceScaleTag.GetTagName(), DanceScale); }
	if (DanceCountTag.IsValid()) { InMap.Add(DanceCountTag.GetTagName(), DanceCount); }
	if (TimeBetweenDanceTag.IsValid()) { InMap.Add(TimeBetweenDanceTag.GetTagName(), TimeBetweenDance); }
}

void UVSAbility_Gundance::NativeLoadDataFromDataManager()
{
	Super::NativeLoadDataFromDataManager();

	ApplyDataFromDataManager(BladeDamageTag, BladeDamage);
	ApplyDataFromDataManager(ShotDamageTag, ShotDamage);
	ApplyDataFromDataManager(DanceScaleTag, DanceScale);
	ApplyDataFromDataManager(DanceCountTag, DanceCount);
	ApplyDataFromDataManager(TimeBetweenDanceTag, TimeBetweenDance);
}

#endif

float UVSAbility_Gundance::GetBladeDamage(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	float ReturnDamage = 0;

#if WITH_EDITOR
	if (ADM == nullptr)
	{
		ReturnDamage = BladeDamage[InLevel];
	}
#endif

	bool bResult = false;

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		ReturnDamage = ADM->FindAbilityData(BladeDamageTag, bResult, InLevel);
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

float UVSAbility_Gundance::GetShotDamage(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	float ReturnDamage = 0;

#if WITH_EDITOR
	if (ADM == nullptr)
	{
		ReturnDamage = ShotDamage[InLevel];
	}
#endif

	bool bResult = false;

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		ReturnDamage = ADM->FindAbilityData(ShotDamageTag, bResult, InLevel);
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

float UVSAbility_Gundance::GetDanceScale(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	float ScaleRate = 1;
	bool bResult = false;

#if WITH_EDITOR
	if (ADM == nullptr && DanceScale.IsValidIndex(InLevel))
	{
		ScaleRate = DanceScale[InLevel];
	}
#endif

#if WITH_EDITOR
	if(ADM != nullptr)
	{
#endif
		ScaleRate = ADM->FindAbilityData(DanceScaleTag, bResult, InLevel);
		if(!bResult) ScaleRate = 1;
#if WITH_EDITOR
	}
#endif

	if (InASC != nullptr)
	{
		float PlayerScaleValue = InASC->GetGameplayAttributeValue(UAttributeSet_Player::GetEffectRangeRateAttribute(), bResult);
		if(!bResult) ScaleRate *= PlayerScaleValue;
	}

	return ScaleRate;
}

float UVSAbility_Gundance::GetDanceCount(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	float ReturnCount = 1;
	bool bResult = false;


#if WITH_EDITOR
	if (ADM == nullptr && DanceCount.IsValidIndex(InLevel))
	{
		ReturnCount = DanceCount[InLevel];
	}
#endif

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		ReturnCount = ADM->FindAbilityData(DanceCountTag, bResult, InLevel);
		if(!bResult) ReturnCount = 1;
#if WITH_EDITOR
	}
#endif

	if (InASC != nullptr)
	{
		float PlayerCountValue = InASC->GetGameplayAttributeValue(UAttributeSet_Player::GetProjectileCountAttribute(), bResult);
		if (!bResult) ReturnCount += PlayerCountValue;
	}

	return ReturnCount;
}

float UVSAbility_Gundance::GetTimeBetweenDance(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	float ReturnTime = 0.25;
	bool bResult = false;


#if WITH_EDITOR
	if (ADM == nullptr && TimeBetweenDance.IsValidIndex(InLevel))
	{
		ReturnTime = TimeBetweenDance[InLevel];
	}
#endif

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		ReturnTime = ADM->FindAbilityData(TimeBetweenDanceTag, bResult, InLevel);
		if(!bResult) ReturnTime = 0.25;
#if WITH_EDITOR
	}
#endif

	return ReturnTime;
}

void UVSAbility_Gundance::ShotAndBlade(int32 ActionNumber)
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if(Avatar == nullptr) return;
	UWorld* InWorld = Avatar->GetWorld();
	check(InWorld)

	FRotator BladeRotator; 
	if(!BladeDegrees.IsEmpty()) { BladeRotator.Yaw  = BladeDegrees[ActionNumber % BladeDegrees.Num()];}
	FTransform BladeSpawnTransform(BladeRotator, Avatar->GetActorLocation(), CachedDanceScale);
	
	AGundanceActor* BladeEffecter = InWorld->SpawnActorDeferred<AGundanceActor>(BladeEffecterClass, BladeSpawnTransform);
	if (BladeEffecter)
	{
		BladeEffecter->AddEffectSpec("Damage", CachedBladeDamageEffectSpecHandle);

		BladeEffecter->FinishSpawning(BladeSpawnTransform);
	}
	
	FRotator ShotRotator;
	if(!ShotDegrees.IsEmpty()) { ShotRotator.Yaw = ShotDegrees[ActionNumber % ShotDegrees.Num()]; }
	FTransform ShotSpawnTransform(ShotRotator, Avatar->GetActorLocation(), CachedDanceScale);

	AGundanceActor* ShotEffecter = InWorld->SpawnActorDeferred<AGundanceActor>(ShotEffecterClass, ShotSpawnTransform);
	if (ShotEffecter)
	{
		ShotEffecter->AddEffectSpec("Damage", CachedShotDamageEffectSpecHandle);

		ShotEffecter->FinishSpawning(ShotSpawnTransform);
	}
}

void UVSAbility_Gundance::OnFinishedGundance(int32 ActionNumber)
{
	ApplyCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

#undef LOCTEXT_NAMESPACE