// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "AbilityDataManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVS_API UAbilityDataManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	static UAbilityDataManager* ADMInstance;

public:
	static UAbilityDataManager* GetAbilityDataManager();


private:
	TMap<FName, TArray<float>> DataMap;

	//CSV테이블 읽기
	void ReadAbilityDataCSV();

	TMap<FName, TSubclassOf<class UVSAbility>> AbilityArray;

	void StartLoadAbilities();
	void CompleteLoadAbilities();

public:
	UFUNCTION(BlueprintPure, Category = "VSAbility")
	TSubclassOf<UVSAbility> FindAbility(const FName& Code) const;

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

public:
	UFUNCTION(BlueprintPure, Category = "VSAbility")
	float FindAbilityData(const FGameplayTag& DataTag, bool& bResult, int Level = 1) const;

	bool InitializeAddableAbilities(const FName& InCharacterName, TArray<FName>& OutAbilities);
};
