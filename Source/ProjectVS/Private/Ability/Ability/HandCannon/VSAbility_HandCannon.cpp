// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/HandCannon/VSAbility_HandCannon.h"
#include "HandCannonFireActor.h"
#include "AbilityDataManager.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet_Player.h"
#include "Ability/Effect/DamageEffect.h"
#include "AttributeSet_Default.h"

#include "ProjectVS.h"

#include "Character/Monster/MonsterWorldManager.h"

#include "Character/Monster/VSMonster.h"

#include "Ability/AbilityBookComponent.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#define LOCTEXT_NAMESPACE "VSAbility"

UVSAbility_HandCannon::UVSAbility_HandCannon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetAbilityType(EVSAbilityType::Active);
}

void UVSAbility_HandCannon::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	AActor* AvatarActor = ActorInfo->AvatarActor.Get();
	if (AvatarActor == nullptr || !CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, false))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}


	UAbilitySystemComponent* InASC = ActorInfo->AbilitySystemComponent.Get();
	
	int AbLevel = GetAbilityLevel(Handle, ActorInfo);

	float FinalFireDamage = GetFireDamage(AbLevel, InASC);
	float FinalFireScale = GetFireScale(AbLevel, InASC);
	int FinalFireCount = GetFireCount(AbLevel, InASC);
	FQuat FireRotation = CreateFireRoation(AvatarActor);

	FireHandCannon(Handle, InASC, FireRotation, FinalFireDamage, FinalFireScale, FinalFireCount);

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

FText UVSAbility_HandCannon::GetLeveUpDescriptionText_Implementation(const UAbilityBookComponent* InBook, int BeforeLevel, int AfterLevel) const
{
	TArray<FText> DescTexts;

	float Value_Before = 0;
	float Value_After = 0;

	UAbilitySystemComponent* ASC = InBook == nullptr ? nullptr : InBook->GetAbilitySystemComponent();

	//발사 데미지
	{
		Value_Before = GetFireDamage(BeforeLevel, ASC);
		Value_After = GetFireDamage(AfterLevel, ASC);
		if (Value_Before != Value_After)
		{
			FFormatOrderedArguments Args;
			Args.Add(Value_Before);
			Args.Add(Value_After);

			DescTexts.Add(FText::Format(LOCTEXT("HandCannon_LevelUp Damage", "발사 데미지 {0} -> {1}"), Args));
		}
	}

	//발사 크기
	{
		Value_Before = GetFireScale(BeforeLevel, ASC);
		Value_After = GetFireScale(AfterLevel, ASC);
		if (Value_Before != Value_After)
		{
			FFormatOrderedArguments Args;
			Args.Add(Value_Before);
			Args.Add(Value_After);

			DescTexts.Add(FText::Format(LOCTEXT("HandCannon_LevelUp Scale", "발사 크기 {0} -> {1}"), Args));
		}
	}

	//발사 회수
	{
		Value_Before = GetFireCount(BeforeLevel, ASC);
		Value_After = GetFireCount(AfterLevel, ASC);
		if (Value_Before != Value_After)
		{
			FFormatOrderedArguments Args;
			Args.Add(Value_Before);
			Args.Add(Value_After);

			DescTexts.Add(FText::Format(LOCTEXT("HandCannon_LevelUp Damage", "발사 회수 {0} -> {1}"), Args));
		}
	}


	return FText::Join(FText::FromString("\n"), DescTexts);
}


FQuat UVSAbility_HandCannon::CreateFireRoation(AActor* AvatarActor) const
{
	if(AvatarActor == nullptr) {return FQuat::Identity;}

	FVector AvatarLocation = AvatarActor->GetActorLocation();

	AVSMonster* Target = nullptr;

	TArray<AVSMonster*> Monsters;

	UMonsterWorldManager* MWM = AvatarActor->GetWorld()->GetSubsystem<UMonsterWorldManager>();

	if(MWM == nullptr) { return FQuat::Identity; }

	MWM->GetMonsters(Monsters);

	if (!Monsters.IsEmpty())
	{
		Target = Monsters[0];
		float MinDist = FVector::DistXY(AvatarLocation, Target->GetActorLocation());
		float TempDist = 0;

		int MaxIdx = Monsters.Num();

		for (int i = 1; i < MaxIdx; i++)
		{
			TempDist = FVector::DistXY(AvatarLocation, Monsters[i]->GetActorLocation());

			if (TempDist < MinDist)
			{
				Target = Monsters[i];
				MinDist = TempDist;
			}
		}
	}

	if (Target == nullptr) return AvatarActor->GetActorQuat();

	FVector TargetLocation = Target->GetActorLocation();

	FVector Dir = TargetLocation - AvatarLocation;
	Dir.Z = 0;

	return Dir.ToOrientationQuat();
}

float UVSAbility_HandCannon::GetFireDamage(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	float ReturnDamage = 0;

#if WITH_EDITOR
	if (ADM == nullptr)
	{
		ReturnDamage = FireDamage[InLevel];
	}
#endif

	bool bResult = false;

#if WITH_EDITOR
	if (ADM != nullptr)
	{
#endif
		ReturnDamage = ADM->FindAbilityData(FireDamageTag, bResult, InLevel);
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

float UVSAbility_HandCannon::GetFireScale(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	float RangeRate = 1;
	bool bResult = false;


#if WITH_EDITOR
	if (ADM == nullptr)
	{
		if(FireScale.IsValidIndex(InLevel)) { RangeRate = FireScale[InLevel]; }
	}
#endif

#if WITH_EDITOR
	if(ADM != nullptr)
	{
#endif
		RangeRate = ADM->FindAbilityData(FireScaleTag, bResult, InLevel);
		if(!bResult) RangeRate = 1;
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

int UVSAbility_HandCannon::GetFireCount(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	int ReturnFireCount = 1;
	bool bResult = false;

#if WITH_EDITOR
	if (ADM == nullptr)
	{
		if(FireCount.IsValidIndex(InLevel)) { ReturnFireCount = FireCount[InLevel]; }
	}
#endif
	
#if WITH_EDITOR
	if(ADM != nullptr)
	{
#endif
		ReturnFireCount = ADM->FindAbilityData(FireCountTag, bResult, InLevel);
		if(!bResult) {ReturnFireCount = 1;}
#if WITH_EDITOR
	}
#endif

	if (InASC != nullptr)
	{
		int ASCCount = InASC->GetGameplayAttributeValue(UAttributeSet_Player::GetProjectileCountAttribute(), bResult);
		if(bResult) { ReturnFireCount += ASCCount; } 
	}

	return ReturnFireCount;
}

#if WITH_EDITOR
/*
EDataValidationResult UVSAbility_HandCannon::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	float TempMaxLevel = GetMaxLevel();


	return 0 < Context.GetNumErrors() ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
*/
void UVSAbility_HandCannon::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PName = PropertyChangedEvent.GetMemberPropertyName();
	if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_HandCannon, FireDamageTag))
	{
		ApplyDataFromDataManager(FireDamageTag, FireDamage);
	}
	else if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_HandCannon, FireScaleTag))
	{
		ApplyDataFromDataManager(FireScaleTag, FireScale);
	}
	else if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_HandCannon, FireCountTag))
	{
		ApplyDataFromDataManager(FireCountTag, FireCount);
	}

}
void UVSAbility_HandCannon::CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const
{
	Super::CreateAbilityDataToSave(InMap);

	if(FireDamageTag.IsValid()){InMap.Add(FireDamageTag.GetTagName(), FireDamage);}
	if(FireScaleTag.IsValid()){InMap.Add(FireScaleTag.GetTagName(), FireScale);}
	if(FireCountTag.IsValid()){InMap.Add(FireCountTag.GetTagName(), FireCount);}
}
void UVSAbility_HandCannon::NativeLoadDataFromDataManager()
{
	Super::NativeLoadDataFromDataManager();

	ApplyDataFromDataManager(FireDamageTag, FireDamage);
	ApplyDataFromDataManager(FireScaleTag, FireScale);
	ApplyDataFromDataManager(FireCountTag, FireCount);
}

#endif

void UVSAbility_HandCannon::FireHandCannon(const FGameplayAbilitySpecHandle Handle, TWeakObjectPtr<class UAbilitySystemComponent> InASC, FQuat Rotation, float Damage, float Scale, int Count)
{
	if(Count <= 0 || !Handle.IsValid() || !InASC.IsValid()) return;

	AActor* AvatarActor = InASC->GetAvatarActor();
	if(AvatarActor == nullptr) return;

	FTransform SpawnTransform = AvatarActor->GetActorTransform();
	SpawnTransform.SetScale3D(FVector(Scale, Scale, 1));
	SpawnTransform.SetRotation(Rotation);

	AHandCannonFireActor* FireActor = AvatarActor->GetWorld()->SpawnActorDeferred<AHandCannonFireActor>(FireActorClass, SpawnTransform);
	if (FireActor)
	{
		FGameplayAbilitySpec* AbilitySpec = InASC->FindAbilitySpecFromHandle(Handle);

		FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
		ContextHandle.SetAbility(this);

		FGameplayEffectSpecHandle FireDamageSpecHandle = InASC->MakeOutgoingSpec(UDamageEffect::StaticClass(), 1, ContextHandle);
		if (FireDamageSpecHandle.IsValid())
		{
			ApplyAbilityTagsToGameplayEffectSpec(*FireDamageSpecHandle.Data.Get(), AbilitySpec);

			// Copy over set by caller magnitudes
			if (AbilitySpec)
			{
				FireDamageSpecHandle.Data->SetByCallerTagMagnitudes = AbilitySpec->SetByCallerTagMagnitudes;
			}
		}

		FireDamageSpecHandle.Data->SetSetByCallerMagnitude(UAttributeSet_Default::GetDamageName(), Damage);

		FireActor->AddEffectSpec("FireDamage", FireDamageSpecHandle);
		FireActor->FinishSpawning(SpawnTransform);
	}

	if (1 < Count)
	{
		FTimerHandle TempTimer;
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &UVSAbility_HandCannon::FireHandCannon, Handle, InASC, Rotation, Damage, Scale, Count-1);
		AvatarActor->GetWorld()->GetTimerManager().SetTimer(TempTimer, TimerDel, 0.1, false);
	}
}

#undef LOCTEXT_NAMESPACE