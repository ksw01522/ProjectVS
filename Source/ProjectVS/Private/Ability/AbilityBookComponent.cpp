// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AbilityBookComponent.h"
#include "ProjectVS.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Ability/VSAbility.h"
#include "Ability/AbilityDataManager.h"

// Sets default values for this component's properties
UAbilityBookComponent::UAbilityBookComponent()
{
	ActivePageSize = 6;
	PassivePageSize = 6;
}

UAbilityBookComponent::~UAbilityBookComponent()
{

}

UAbilitySystemComponent* UAbilityBookComponent::GetAbilitySystemComponent() const
{
	return WeakASC.Get();
}

void UAbilityBookComponent::RegisterAbilitySystem(UAbilitySystemComponent* InASC)
{
	//Check Valid
	if (InASC == nullptr)
	{
		LOG_ERROR(TEXT("Try Register {NULL} ASC In Book."));
		return;
	}
	else if (InASC == WeakASC)
	{
		LOG_ERROR(TEXT("Try Register {Same} ASC In Book"));
		return;
	}

	UnregisterAbilitySystem(WeakASC.Get());

	WeakASC = InASC;

	//Give Ability Page
	for (auto& Page : ActivePages)
	{
		GiveAbility(*Page);
	}

	for (auto& Page : PassivePages)
	{
		GiveAbility(*Page);
	}
}

void UAbilityBookComponent::UnregisterAbilitySystem(UAbilitySystemComponent* InASC)
{
	if (InASC != WeakASC || InASC == nullptr || !WeakASC.IsValid())
	{
		return;
	}

	//Clear Gived Ability Page
	for (auto& Page : ActivePages)
	{
		ClearAbility(*Page);
	}

	for (auto& Page : PassivePages)
	{
		ClearAbility(*Page);
	}

	WeakASC.Reset();
}

void UAbilityBookComponent::ClearAbilityBook()
{
	UnregisterAbilitySystem(WeakASC.Get());

	ActivePages.Empty();
	PassivePages.Empty();
	AddableAbilities.Empty();
	BanndedAbilities.Empty();
}


void UAbilityBookComponent::SetOrAddPageByLevelUp(FName InCode, int NewLevel)
{
	const UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();
	TSubclassOf<UVSAbility> FindedAbility = ADM->FindAbility(InCode);
	const UVSAbility* AbObj = StaticCast<UVSAbility*>(FindedAbility->GetDefaultObject());

	if (AbObj == nullptr) return;

	int CurrentLevel = GetAbilityLevel(InCode);
	if(CurrentLevel == 0 && 0 < NewLevel) { AbObj->PreAddInBook(this); }	

	SetOrAddPage(InCode, NewLevel, false);
}

void UAbilityBookComponent::SetOrAddPage(FName InCode, int NewLevel, bool bForced)
{
	const UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();
	TSubclassOf<UVSAbility> FindedAbility = ADM->FindAbility(InCode);
	const UVSAbility* AbObj = StaticCast<UVSAbility*>(FindedAbility->GetDefaultObject());

	if (AbObj == nullptr) return;

	if (NewLevel <= 0) {
		RemovePage(InCode);
		return;
	}

	if (FAbilityPage* FindedPage = FindAbilityPage(InCode, AbObj->GetAbilityType()))
	{
		if (FindedPage->Level == NewLevel)
		{
			return;
		}

		FindedPage->Level = NewLevel;

		ClearAbility(*FindedPage);
		GiveAbility(*FindedPage);

		return;
	}

	FAbilityPage* NewPage = new FAbilityPage(InCode, NewLevel);
	AddableAbilities.Remove(InCode);

	TArray<TSharedPtr<FAbilityPage>>& TargetPages = AbObj->GetAbilityType() == EVSAbilityType::Active ? ActivePages : PassivePages;

	TargetPages.Emplace(NewPage);

	GiveAbility(*NewPage);

	if (OnUpdateBookDelegate.IsBound())
	{
		OnUpdateBookDelegate.Broadcast(this);
	}
}



int UAbilityBookComponent::GetAbilityLevel(const FName& InCode) const
{
	for (auto& Page : ActivePages)
	{
		if (Page->Code == InCode)
		{
			return Page->GetAbilityLevel();
		}
	}

	for (auto& Page : PassivePages)
	{
		if (Page->Code == InCode)
		{
			return Page->GetAbilityLevel();
		}
	}
	return 0;
}


void UAbilityBookComponent::SetAbilityPageSize(EVSAbilityType Type, int NewSize)
{
	ensure(Type != EVSAbilityType::None);

	TArray<TSharedPtr<FAbilityPage>>& TargetPages = Type == EVSAbilityType::Active ? ActivePages : PassivePages;
	int& TargetSize = Type == EVSAbilityType::Active ? ActivePageSize : PassivePageSize;

	TargetSize = NewSize;

	TargetPages.Reserve(NewSize);
}

void UAbilityBookComponent::GiveAbility(FAbilityPage& InPage)
{
	if(!WeakASC.IsValid()) return;

	const UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	TSubclassOf<UVSAbility> FindedClass = ADM->FindAbility(InPage.Code);

	FGameplayAbilitySpec AbilitySpec(FindedClass, InPage.Level);
	InPage.Handle = WeakASC->GiveAbility(AbilitySpec);
}

void UAbilityBookComponent::ClearAbility(FAbilityPage& InPage)
{
	if (!WeakASC.IsValid()) return;

	WeakASC->ClearAbility(InPage.Handle);
	InPage.Handle = FGameplayAbilitySpecHandle();
}

bool UAbilityBookComponent::IsBookFullPage(EVSAbilityType Type) const
{
	const TArray<TSharedPtr<FAbilityPage>>& TargetPages = Type == EVSAbilityType::Active ? ActivePages : PassivePages;
	int MaxSize = Type == EVSAbilityType::Active ? ActivePageSize : PassivePageSize;

	return MaxSize <= TargetPages.Num();
}

FAbilityPage* UAbilityBookComponent::FindAbilityPage(const FName& InCode) const
{
	FAbilityPage* ReturnPage = FindAbilityPage(InCode, EVSAbilityType::Active);
	if (ReturnPage != nullptr)
	{
		return ReturnPage;
	}
	
	ReturnPage = FindAbilityPage(InCode, EVSAbilityType::Passive);
	return ReturnPage;
}

void UAbilityBookComponent::GetLevelUpTargetArray(TArray<FAbilityLevelUpTargetInfo>& OutResult, int Count, int TargetLevel) const
{
	//Code, CurrentLevel, TargetLevel, 순서를 위한 값
	TArray<TPair<int, FAbilityLevelUpTargetInfo>> TempArray;
	TempArray.Reserve(Count);

	const UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();

	//레벨업 우선순위 체크
	for (const auto& Page : ActivePages)
	{
		int Priority = FMath::RandRange(0, 100);
		TPair<int, FAbilityLevelUpTargetInfo>* MinPriorityInfo = nullptr;

		if (Count <= TempArray.Num())
		{
			for(auto& Ele : TempArray)
			{
				if(MinPriorityInfo == nullptr || Ele.Key < MinPriorityInfo->Key) 
				{MinPriorityInfo = &Ele;}
			}

			if(Priority <= MinPriorityInfo->Key) {continue;}
		}

		const FName& AbilityCode = Page->Code;

		UVSAbility* TempObj = StaticCast<UVSAbility*>(ADM->FindAbility(AbilityCode)->GetDefaultObject());

		int CurrentLv = Page->Level;

		int TargetLv = FMath::Min<int>(CurrentLv + TargetLevel, TempObj->GetMaxLevel());

		//책안에 설정될 수 있는지
		if(!TempObj->CanSetOrAddInBook(this, TargetLv)) {continue;}

		if (TempArray.Num() < Count)
		{
			TempArray.Emplace(Priority, FAbilityLevelUpTargetInfo(AbilityCode, CurrentLv, TargetLv));
		}
		else
		{
			MinPriorityInfo->Key = Priority;

			MinPriorityInfo->Value.Code = AbilityCode;
			MinPriorityInfo->Value.CurrentLevel = CurrentLv;
			MinPriorityInfo->Value.TargetLevel = TargetLv;
		}
	}
	
	for (const auto& Page : PassivePages)
	{
		int Priority = FMath::RandRange(0, 100);
		TPair<int, FAbilityLevelUpTargetInfo>* MinPriorityInfo = nullptr;

		if (Count <= TempArray.Num())
		{
			for (auto& Ele : TempArray)
			{
				if (MinPriorityInfo == nullptr || Ele.Key < MinPriorityInfo->Key)
				{
					MinPriorityInfo = &Ele;
				}
			}

			if (Priority <= MinPriorityInfo->Key) { continue; }
		}

		const FName& AbilityCode = Page->Code;

		UVSAbility* TempObj = StaticCast<UVSAbility*>(ADM->FindAbility(AbilityCode)->GetDefaultObject());

		int CurrentLv = Page->Level;

		int TargetLv = FMath::Min<int>(CurrentLv + TargetLevel, TempObj->GetMaxLevel());

		//책안에 설정될 수 있는지
		if (!TempObj->CanSetOrAddInBook(this, TargetLv)) { continue; }

		if (TempArray.Num() < Count)
		{
			TempArray.Emplace(Priority, FAbilityLevelUpTargetInfo(AbilityCode, CurrentLv, TargetLv));
		}
		else
		{
			MinPriorityInfo->Key = Priority;

			MinPriorityInfo->Value.Code = AbilityCode;
			MinPriorityInfo->Value.CurrentLevel = CurrentLv;
			MinPriorityInfo->Value.TargetLevel = TargetLv;
		}
	}

	for (const auto& InCode : AddableAbilities)
	{
		int Priority = FMath::RandRange(0, 100);
		TPair<int, FAbilityLevelUpTargetInfo>* MinPriorityInfo = nullptr;

		if (Count <= TempArray.Num())
		{
			for (auto& Ele : TempArray)
			{
				if (MinPriorityInfo == nullptr || Ele.Key < MinPriorityInfo->Key)
				{
					MinPriorityInfo = &Ele;
				}
			}

			if (Priority <= MinPriorityInfo->Key) { continue; }
		}

		UVSAbility* TempObj = StaticCast<UVSAbility*>(ADM->FindAbility(InCode)->GetDefaultObject());

		int CurrentLv = 0;

		int TargetLv = FMath::Min<int>(CurrentLv + TargetLevel, TempObj->GetMaxLevel());

		//책안에 설정될 수 있는지
		if (!TempObj->CanSetOrAddInBook(this, TargetLv)) { continue; }

		if (TempArray.Num() < Count)
		{
			TempArray.Emplace(Priority, FAbilityLevelUpTargetInfo(InCode, CurrentLv, TargetLv));
		}
		else
		{
			MinPriorityInfo->Key = Priority;

			MinPriorityInfo->Value.Code = InCode;
			MinPriorityInfo->Value.CurrentLevel = CurrentLv;
			MinPriorityInfo->Value.TargetLevel = TargetLv;
		}
	}

	for (auto& Ele : TempArray)
	{
		OutResult.Add(Ele.Value);
	}
}

void UAbilityBookComponent::InitializeAddableAbilities(const FName& InCharacterName)
{
	UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();
	if (ADM == nullptr) {return;}

	AddableAbilities.Empty();
	ADM->InitializeAddableAbilities(InCharacterName, AddableAbilities);
}

void UAbilityBookComponent::GetAbilityPageArray(TArray<const FAbilityPage*>& OutPage, EVSAbilityType InType) const
{
	check(InType != EVSAbilityType::None)

	const TArray<TSharedPtr<FAbilityPage>>& TargetPages = InType == EVSAbilityType::Active ? ActivePages : PassivePages;
	LOG_ERROR(TEXT("%d"), TargetPages.Num());

	for (auto& Target : TargetPages)
	{
		OutPage.Add(Target.Get());
	}
}



FAbilityPage* UAbilityBookComponent::FindAbilityPage(const FName& InCode, EVSAbilityType InType) const
{
	const TArray<TSharedPtr<FAbilityPage>>& TargetPages = InType == EVSAbilityType::Active ? ActivePages : PassivePages;

	for (auto& Page : TargetPages)
	{
		if (Page->Code == InCode)
		{
			return Page.Get();
		}
	}

	return nullptr;
}

void UAbilityBookComponent::RemovePage(const FName& InCode)
{
	int MaxIDX = ActivePages.Num();

	for (int i = 0; i < MaxIDX; i++)
	{
		if (ActivePages[i]->Code == InCode)
		{
			ClearAbility(*ActivePages[i]);
			ActivePages.RemoveAt(i, EAllowShrinking::No);
			return;
		}
	}

	MaxIDX = PassivePages.Num();

	for (int i = 0; i < MaxIDX; i++)
	{
		if (PassivePages[i]->Code == InCode)
		{
			ClearAbility(*PassivePages[i]);
			PassivePages.RemoveAt(i, EAllowShrinking::No);
			return;
		}
	}
}

void UAbilityBookComponent::BanAbility(FName InCode)
{
	AddableAbilities.Remove(InCode);
	BanndedAbilities.Add(InCode);
}

void UAbilityBookComponent::BanAbility(const TArray<FName>& InCodes)
{
	for (auto& InCode : InCodes)
	{
		BanAbility(InCode);
	}
}





