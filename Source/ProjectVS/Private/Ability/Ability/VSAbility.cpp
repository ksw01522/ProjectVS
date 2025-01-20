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


	LOG_ERROR(TEXT("Call PostCDOCompiled"));
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

float UVSAbility::GetCooldownTime(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const
{
	int CurrentLevel = GetAbilityLevel(Handle, ActorInfo);
	UAbilityDataManager* DataManager = UAbilityDataManager::GetAbilityDataManager();
	
#if WITH_EDITOR
	if(DataManager==nullptr)
	{
		return CoolTime[CurrentLevel - 1];
	}
#endif
	
	bool bResult;
	return DataManager->FindAbilityData(CoolTimeTag, bResult, CurrentLevel);
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

	float CurrentCoolTime = GetCooldownTime(Handle, ActorInfo);
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

bool UVSAbility::CanSetOrAddInBook(const UAbilityBookComponent& InBook, int NewLevel) const
{
	if (NewLevel <= 0) return false;

	if (int CurrentLevel = InBook.GetAbilityLevel(GetAbilityCode()); 0 < CurrentLevel)
	{
		return CurrentLevel < NewLevel;
	}
	else
	{
		return !InBook.IsBookFullPage(GetAbilityType());
	}
}

FText UVSAbility::GetDescriptionText_Implementation() const
{
	return FText::GetEmpty();
}

FText UVSAbility::GetLeveUpDescriptionText_Implementation(int BeforeLevel, int AfterLevel) const
{
	return FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE