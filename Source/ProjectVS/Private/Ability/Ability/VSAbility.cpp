// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/VSAbility.h"
#include "AbilitySystemComponent.h"
#include "ProjectVS.h"
#include "Ability/AbilityDataManager.h"
#include "Ability/Effect/CooldownEffect.h"
#include "Ability/AbilityBookComponent.h"
#include "Misc/PackageName.h"
#include "UObject/ObjectSaveContext.h"
#include "UObject/Package.h"
#include "Engine/AssetManager.h"
#include "AbilitySystemLog.h"
#include "Ability/AttributeSet_Player.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#include "AbilityEditorSubsystem.h"
#endif

#define LOCTEXT_NAMESPACE "VSAbility"

UVSAbility::UVSAbility(const FObjectInitializer& ObjectInitializer)	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::NonInstanced;

	AbilityType = EVSAbilityType::None;

	CooldownGameplayEffectClass = UCooldownEffect::StaticClass();

	MaxLevel = 1;

	SourceBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
}

FPrimaryAssetId UVSAbility::GetPrimaryAssetId() const
{
	FPrimaryAssetType AssetType = GetAbilityAssetType();

	return FPrimaryAssetId(AssetType, AbilityCode);
}

FPrimaryAssetType UVSAbility::GetAbilityAssetType()
{
	return FPrimaryAssetType("VSAbility");
}

void UVSAbility::PostLoad()
{
	Super::PostLoad();

#if WITH_EDITORONLY_DATA
	FAssetBundleData OldData = AssetBundleData;

	UpdateAssetBundleData();

	if (UAssetManager::IsInitialized() && OldData != AssetBundleData)
	{
		// Bundles changed, refresh
		UAssetManager::Get().RefreshAssetData(this);
	}
#endif

	LOG_ERROR(TEXT("Call PostLoad"));

#if WITH_EDITOR
	if (GetFlags() || EObjectFlags::RF_ClassDefaultObject)
	{
		LoadDataFromDataManager();
	}

#endif
}

#if WITH_EDITOR
void UVSAbility::UpdateAssetBundleData()
{
	if (UAssetManager::IsInitialized())
	{
		AssetBundleData.Reset();
		UAssetManager::Get().InitializeAssetBundlesFromMetadata(this, AssetBundleData);
	}
}

void UVSAbility::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
	Super::PreSave(ObjectSaveContext);

	UpdateAssetBundleData();

	if (UAssetManager::IsInitialized())
	{
		// Bundles may have changed, refresh
		UAssetManager::Get().RefreshAssetData(this);
	}

	LOG_ERROR(TEXT("Call PreSave"));

#if WITH_EDITOR
	//Editor to CSV Write
	/*UAbilityEditorSubsystem* AES = UAbilityEditorSubsystem::GetAbilityEditorSubsystem();

	TMap<FName, TArray<float>> DataMapToSave;
	CreateAbilityDataToSave(DataMapToSave);

	AES->AddAbilityData(DataMapToSave);*/

#endif
}

#endif

void UVSAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
}

void UVSAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (TriggerEventData && bHasBlueprintActivateFromEvent)
	{
		// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
		K2_ActivateAbilityFromEvent(*TriggerEventData);
	}
	else if (bHasBlueprintActivate)
	{
		// A Blueprinted ActivateAbility function must call CommitAbility somewhere in its execution chain.
		K2_ActivateAbility();
	}
	else if (bHasBlueprintActivateFromEvent)
	{
		UE_LOG(LogAbilitySystem, Warning, TEXT("Ability %s expects event data but none is being supplied. Use 'Activate Ability' instead of 'Activate Ability From Event' in the Blueprint."), *GetName());
		constexpr bool bReplicateEndAbility = false;
		constexpr bool bWasCancelled = true;
		EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
	else
	{
		ActivateAbility_CPP(Handle, ActorInfo, ActivationInfo,TriggerEventData);
	}
}

void UVSAbility::ActivateAbility_CPP(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	LOG_ERROR(TEXT("Must Write ActivateAbility Function"));
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);

}


void UVSAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

#if WITH_EDITOR
void UVSAbility::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PName = PropertyChangedEvent.GetMemberPropertyName();
	if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility, MaxLevelTag))
	{ 
		ApplyDataFromDataManager(MaxLevelTag, MaxLevel);
	}
	else if (PName == GET_MEMBER_NAME_CHECKED(UVSAbility, CoolTimeTag))
	{
		CachedCooldownTags.Reset();
		ApplyDataFromDataManager(CoolTimeTag, CoolTime);
	}
}

void UVSAbility::LoadDataFromDataManager()
{
	NativeLoadDataFromDataManager();
}

void UVSAbility::NativeLoadDataFromDataManager()
{
	ApplyDataFromDataManager(MaxLevelTag, MaxLevel);
	ApplyDataFromDataManager(CoolTimeTag, CoolTime);
}

void UVSAbility::ApplyDataFromDataManager(const FGameplayTag& InTag, float& InData)
{
	if (UAbilityEditorSubsystem* UES = UAbilityEditorSubsystem::GetAbilityEditorSubsystem())
	{
		bool bMustMarkDirty = false;
		bool bFound = false;
		float Result = 0;
		Result = UES->FindAbilityData(InTag, bFound);

		if (bFound && InData != Result)
		{
			InData = Result;
			bMustMarkDirty = true;
		}

		if (bMustMarkDirty)
		{
			UPackage* Pack = GetPackage();
			Pack->SetDirtyFlag(true);
		}
	}
}

void UVSAbility::ApplyDataFromDataManager(const FGameplayTag& InTag, TArray<float>& InData)
{
	if (UAbilityEditorSubsystem* UES = UAbilityEditorSubsystem::GetAbilityEditorSubsystem())
	{
		TArray<float> TempArray;
		bool bFound = UES->FindAbilityDataArray(TempArray, InTag);

		bool bMustMarkDirty = false;

		if (bFound && TempArray != InData)
		{
			InData = TempArray;
			bMustMarkDirty = true;
		}

		if (bMustMarkDirty)
		{
			UPackage* Pack = GetPackage();
			Pack->SetDirtyFlag(true);
		}
	}
}


#endif

float UVSAbility::GetMaxLevel() const
{
	UAbilityDataManager* DataManager = UAbilityDataManager::GetAbilityDataManager();

#if WITH_EDITOR
	if (DataManager == nullptr)
	{
		return MaxLevel;
	}
#endif

	bool bResult;
	return DataManager->FindAbilityData(MaxLevelTag, bResult);
}

#if WITH_EDITOR
void UVSAbility::CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const
{
	//MaxLevel
	if(MaxLevelTag.IsValid()) {InMap.Add(MaxLevelTag.GetTagName(), {MaxLevel});}

	//CoolTime
	if(CoolTimeTag.IsValid()) {InMap.Add(CoolTimeTag.GetTagName(), CoolTime); }
}

void UVSAbility::PostCDOCompiled(const FPostCDOCompiledContext& Context)
{
	Super::PostCDOCompiled(Context);

}

EDataValidationResult UVSAbility::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	if (AbilityType == EVSAbilityType::None)
	{
		FText TypeErrorText = FText::FromString("Ability Type Is None");
		Context.AddError(TypeErrorText);
	}



	return 0 < Context.GetNumErrors() ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}



#endif

float UVSAbility::GetCooldownTime_Implementation(int InLevel, UAbilitySystemComponent* InASC) const
{
	UAbilityDataManager* DataManager = UAbilityDataManager::GetAbilityDataManager();
	
#if WITH_EDITOR
	if(DataManager == nullptr)
	{
		return CoolTime[InLevel];
	}
#endif
	
	bool bResult;
	float ReturnCoolTime = DataManager->FindAbilityData(CoolTimeTag, bResult, InLevel);

	if (InASC != nullptr)
	{
		float CoolTimeRate = InASC->GetGameplayAttributeValue(UAttributeSet_Player::GetCoolTimeRateAttribute(), bResult);
		if(bResult) {ReturnCoolTime *= CoolTimeRate;}
	}

	return ReturnCoolTime;
}

void UVSAbility::SetCooldownTag(const FGameplayTag& NewTag)
{
	CoolTimeTag = NewTag;
}

const FGameplayTagContainer* UVSAbility::GetCooldownTags() const
{
	if(!CachedCooldownTags.IsEmpty()) return &CachedCooldownTags;

	const FGameplayTagContainer* SuperTags = Super::GetCooldownTags();
	if (SuperTags)
	{
		CachedCooldownTags.AppendTags(*SuperTags);
	}
	CachedCooldownTags.AddTag(CoolTimeTag);

	return &CachedCooldownTags;
}

void UVSAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE == nullptr) return;

	UAbilitySystemComponent* InASC = ActorInfo->AbilitySystemComponent.Get();
	int AbLevel = GetAbilityLevel(Handle, ActorInfo);

	float CurrentCoolTime = GetCooldownTime(AbLevel, InASC);
	if(CurrentCoolTime <= 0) return;

	int BeforC = ActorInfo->AbilitySystemComponent->GetTagCount(CoolTimeTag);

	FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, CooldownGE->GetClass());
	SpecHandle.Data->DynamicGrantedTags.AddTag(CoolTimeTag);
	SpecHandle.Data->SetSetByCallerMagnitude(UCooldownEffect::CoolTimeSetByCallerName, CurrentCoolTime);

	FActiveGameplayEffectHandle ActiveHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	
	int AfterC = ActorInfo->AbilitySystemComponent->GetTagCount(CoolTimeTag);

	bool bMustAddCooldownEvent = false;

	for (const auto& Trigger : AbilityTriggers)
	{
		if (Trigger.TriggerSource == EGameplayAbilityTriggerSource::GameplayEvent && Trigger.TriggerTag == CoolTimeTag)
		{
			bMustAddCooldownEvent = true;
			break;
		}
	}
	
	if (bMustAddCooldownEvent && ActiveHandle.IsValid())
	{
		TWeakObjectPtr<UAbilitySystemComponent> WeakASC = ActorInfo->AbilitySystemComponent;
		
		WeakASC->GetActiveEffectEventSet(ActiveHandle)->OnEffectRemoved.AddLambda([WeakASC, TempCooldownTag = CoolTimeTag](const FGameplayEffectRemovalInfo& Info){
			if (WeakASC.IsValid())
			{
				FScopedPredictionWindow NewScopedWindow(WeakASC.Get(), true);

				FGameplayEventData EventData;
				WeakASC->HandleGameplayEvent(TempCooldownTag, &EventData);
			}
		});
	}
}

void UVSAbility::PreAddInBook_Implementation(UAbilityBookComponent* InBook) const
{
	check(InBook)
	LOG_WARNING(TEXT("Call Pre Added In Book"));
}

bool UVSAbility::CanSetOrAddInBook_Implementation(const UAbilityBookComponent* InBook, int NewLevel) const
{
	check(InBook)
	if (NewLevel < 0) return false;

	if (int CurrentLevel = InBook->GetAbilityLevel(GetAbilityCode()); 0 < CurrentLevel)
	{
		return CurrentLevel < NewLevel;
	}
	else
	{
		return !InBook->IsBookFullPage(GetAbilityType());
	}
}

FText UVSAbility::GetDescriptionText_Implementation() const
{
	return FText::GetEmpty();
}

FText UVSAbility::GetLeveUpDescriptionText_Implementation(const UAbilityBookComponent* InBook, int BeforeLevel, int AfterLevel) const
{
	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE