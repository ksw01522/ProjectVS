// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "AbilityEditorSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class ABILITYTESTEREDITOR_API UAbilityEditorSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
	
public:
	UAbilityEditorSubsystem();

private:

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

private:
	static TObjectPtr<UAbilityEditorSubsystem> StaticAESInstance;

public:
	static UAbilityEditorSubsystem* GetAbilityEditorSubsystem();

private:
	TMap<FName, TMap<FName, TArray<float>>> DataMap;

	//어빌리티 데이터 CSV테이블 폴더 경로
	FString RootFolderPath;

	//CSV테이블 읽기/쓰기
	void ReadAbilityDataCSV();
	void WriteAbilityDataCSV() const;

	
	void StartLoadAbilitiesForApplyData() const;

	void CompleteLoadAbilitiesForAplyData() const;

public:
	void AddAbilityData(const TMap<FName, TArray<float>>& NewData);

	float FindAbilityData(const struct FGameplayTag& InTag, bool& bFinded, int Level = 1) const;
	bool FindAbilityDataArray(TArray<float>& Out, const struct FGameplayTag& InTag) const;

	void ReReadAbilityDataCSV();

}; 
