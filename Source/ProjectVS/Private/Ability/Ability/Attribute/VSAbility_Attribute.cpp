// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Attribute/VSAbility_Attribute.h"
#include "AbilitySystemComponent.h"
#include "Ability/AbilityDataManager.h"

UVSAbility_Attribute::UVSAbility_Attribute(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetAbilityType(EVSAbilityType::Passive);
	SetAbilityCategory("AttributeBonus");
}

void UVSAbility_Attribute::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();

	if (ASC)
	{
		float FinalValue = GetValue(GetAbilityLevel(Handle, ActorInfo));

		ASC->ApplyModToAttribute(TargetAttribute, ModifierOp, FinalValue);
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

}

void UVSAbility_Attribute::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	
	if (ASC != nullptr && !bWasCancelled)
	{
		float FinalValue = GetValue(GetAbilityLevel(Handle, ActorInfo));
		TEnumAsByte<EGameplayModOp::Type> TargetModOp = ModifierOp;

		if(ModifierOp == EGameplayModOp::Additive)
		{
			FinalValue *= -1;
		}
		else if (ModifierOp == EGameplayModOp::Multiplicitive)
		{
			TargetModOp = EGameplayModOp::Division;
		}
		else if (ModifierOp == EGameplayModOp::Division)
		{
			TargetModOp = EGameplayModOp::Multiplicitive;
		}
		

		ASC->ApplyModToAttribute(TargetAttribute, TargetModOp, FinalValue);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#if WITH_EDITOR
void UVSAbility_Attribute::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PName = PropertyChangedEvent.GetPropertyName();
	if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility_Attribute, BonuseValueTag))
	{
		ApplyDataFromDataManager(BonuseValueTag, BonusValue);
	}
}

void UVSAbility_Attribute::CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const
{
	Super::CreateAbilityDataToSave(InMap);

	if(BonuseValueTag.IsValid()) InMap.Add(BonuseValueTag.GetTagName(), BonusValue);
}

void UVSAbility_Attribute::NativeLoadDataFromDataManager()
{
	Super::NativeLoadDataFromDataManager();

	ApplyDataFromDataManager(BonuseValueTag, BonusValue);
}

#endif

float UVSAbility_Attribute::GetValue(int InAbilityLevel) const
{
	if(InAbilityLevel <= 0) return 0;

	float RetValue = 0;

	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();
	if (ADM)
	{
		bool bResult;
		RetValue = ADM->FindAbilityData(BonuseValueTag, bResult, InAbilityLevel);
	}
#if WITH_EDITOR
	else{ if(BonusValue.IsValidIndex(InAbilityLevel)){RetValue = BonusValue[InAbilityLevel];}}
#endif

	return RetValue;
}

FText UVSAbility_Attribute::GetDescriptionText_Implementation() const
{
	FFormatNamedArguments Args;

	Args.Add("AttributeName", FText::FromString(TargetAttribute.GetName()));

	return FText::Format(NSLOCTEXT("VSAbility", "AttributeAbilityDesc", "{AttributeName}의 추가 스테이터스입니다."), Args);
}

FText UVSAbility_Attribute::GetLeveUpDescriptionText_Implementation(int BeforeLevel, int AfterLevel) const
{
	FFormatNamedArguments Args;

	Args.Add("AttributeName", FText::FromString(TargetAttribute.GetName()));
	Args.Add("BeforeValue", GetValue(BeforeLevel));
	Args.Add("AfterValue", GetValue(AfterLevel));

	return FText::Format(NSLOCTEXT("VSAbility", "AttributeAbilityDesc", "{AttributeName}가 {BeforeValue} -> {AfterValue} 증가합니다."), Args);
}
