// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CharacterDataManager.generated.h"

USTRUCT()
struct FMonsterDataStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float HP_Base = 0;

	UPROPERTY(EditAnywhere)
	float HP_Level = 0;

	UPROPERTY(EditAnywhere)
	float ATK_Base = 0;

	UPROPERTY(EditAnywhere)
	float ATK_Level = 0;

	UPROPERTY(EditAnywhere)
	float MoveSpeed = 0;

	UPROPERTY(EditAnywhere)
	float BountyEXP = 0;

	UPROPERTY(EditAnywhere)
	float BountyGold = 0;

	UPROPERTY(EditAnywhere)
	float BodyTackleDamage = 0;
};

struct FLoadedMonsterDataStruct
{
	FLoadedMonsterDataStruct() : Data(nullptr)
	{

	}

	~FLoadedMonsterDataStruct()
	{
		if(Data != nullptr) delete Data;
	}

	FMonsterDataStruct* Data;
	TSubclassOf<class UVSMonster> MonsterClass; 
};

/**
 * 
 */
UCLASS()
class PROJECTVS_API UCharacterDataManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	static UCharacterDataManager* DataManager;

public:
	static const UCharacterDataManager* GetCharacterDataManager();

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

private:
	UPROPERTY()
	TObjectPtr<class UDataTable> MonsterDataTable;

	UPROPERTY()
	TMap<FName, TSubclassOf<class AVSMonster>> MonsterClassTable;

	FString GetDataTablePath() const;

	void ReadMonsterDataTableCSV();

	void CreateMonsterClassTable();

	UPROPERTY()
	TMap<FName, const class UPlayableCharacterDataAsset*> PlayableCharacterAssetMap;

	void StartLoadPlayableCharacterAssets();
	void CompleteLoadPlayableCharacterAssets();

public:
	TSubclassOf<AVSMonster> GetMonsterClass(const FName& InCode) const;

	const FMonsterDataStruct* FindMonsterData(const FName& InCode) const;

	const UPlayableCharacterDataAsset* FindPlayableCharacterData(const FName& InName) const;
};
