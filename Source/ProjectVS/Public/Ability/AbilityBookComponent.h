// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "AbilitySystemInterface.h"
#include "AbilityBookComponent.generated.h"

class UAbilityBookComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateAbilityBook, UAbilityBookComponent*, InBook);


enum class EVSAbilityType : uint8;

USTRUCT()
struct FAbilityPage
{
	GENERATED_BODY();

	friend class UAbilityBookComponent;

	FAbilityPage()
	{}
	
private:
	FAbilityPage(const FName& InCode, int InLevel) : Code(InCode), Level(InLevel)
	{}
private:
	FName Code;

	FGameplayAbilitySpecHandle Handle;

	int Level = 1;

public:
	FName GetAbilityCode() const { return Code; }
	int GetAbilityLevel() const { return Level; }
};

USTRUCT()
struct FAbilityLevelUpTargetInfo
{
	GENERATED_BODY()

	FAbilityLevelUpTargetInfo()
	{}
	virtual ~FAbilityLevelUpTargetInfo()
	{}

	FAbilityLevelUpTargetInfo(const FName& InCode, const int& InCurLv, const int& InTargetLv) 
		: Code(InCode), CurrentLevel(InCurLv), TargetLevel(InTargetLv)
	{}

	UPROPERTY()
	FName Code;

	UPROPERTY()
	int CurrentLevel = 0;

	UPROPERTY()
	int TargetLevel = 0;
};

UCLASS()
class PROJECTVS_API UAbilityBookComponent : public UActorComponent, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilityBookComponent();
	virtual ~UAbilityBookComponent();

private:
	TArray<FName> AddableAbilities;
	TArray<FName> BanndedAbilities;

	TArray<TSharedPtr<FAbilityPage>> ActivePages;

	int ActivePageSize;

	TArray<TSharedPtr<FAbilityPage>> PassivePages;

	int PassivePageSize;

	TWeakObjectPtr<class UAbilitySystemComponent> WeakASC;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	mutable FOnUpdateAbilityBook OnUpdateBookDelegate;

public:
	FOnUpdateAbilityBook& GetOnUpdateBookDelegate() const { return OnUpdateBookDelegate; }

private:
	void SetAbilityPageSize(EVSAbilityType Type, int NewSize);

	void GiveAbility(FAbilityPage& InPage);
	void ClearAbility(FAbilityPage& InPage);

	void RemovePage(const FName& InCode);

	void BanAbility(FName InCode);
	void BanAbility(const TArray<FName>& InCodes);

public:
	void SetOrAddPageByLevelUp(FName InCode, int NewLevel);

	void SetOrAddPage(FName InCode, int NewLevel, bool bForced = false);

	int GetAbilityLevel(const FName& InCode) const;

protected:

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void RegisterAbilitySystem(UAbilitySystemComponent* InASC);
		
	void UnregisterAbilitySystem(UAbilitySystemComponent* InASC);

	void ClearAbilityBook();

	bool IsBookFullPage(EVSAbilityType Type) const;

	FAbilityPage* FindAbilityPage(const FName& InCode) const;
	FAbilityPage* FindAbilityPage(const FName& InCode, EVSAbilityType InType) const;

	void GetLevelUpTargetArray(TArray<FAbilityLevelUpTargetInfo>& OutResult, int Count, int TargetLevel = 1) const;

	void InitializeAddableAbilities(const FName& InCharacterName);

	void GetAbilityPageArray(TArray<const FAbilityPage*>& OutPage, EVSAbilityType InType) const;
};
