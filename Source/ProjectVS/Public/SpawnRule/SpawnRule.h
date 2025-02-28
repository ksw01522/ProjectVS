// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpawnRule.generated.h"


USTRUCT(BlueprintType)
struct FSpawnData
{
	GENERATED_BODY()

	FSpawnData() : Count(1), Level(1)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
	FName Code;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster", meta = (ClampMin = "1"))
	int Count;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MonsterSpawn", meta = (AllowPrivateAccess = "true", ClampMin = "1"))
	int Level;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MonsterSpawn", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	int LevelByTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "MonsterSpawn", meta = (AllowPrivateAccess = "true", ClampMin = "0"))
	int BetweenTime;
};
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced, Abstract)
class PROJECTVS_API USpawnRule : public UObject
{
	GENERATED_BODY()
	
public:
	USpawnRule();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "MonsterSpawn", meta = (AllowPrivateAccess = "true"))
	TArray<FSpawnData> MonsterSpawnDatas;

private:
	virtual void CreateSpawnLocations(TArray<FVector>& OutLocations, const FSpawnData& SpawnData, class UWorld& InWorld);

public:
	void SpawnMonster(class UWorld& InWorld);
};


UCLASS(DisplayName = "RandomInDonut")
class PROJECTVS_API USpawnRule_RandomInDonut : public USpawnRule
{
	GENERATED_BODY()

public:
	USpawnRule_RandomInDonut();

#if WITH_EDITOR
protected:
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;
#endif

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterSpawn", meta = (AllowPrivateAccess = true, ClampMin = "0"))
	float InnerCircleRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MonsterSpawn", meta = (AllowPrivateAccess = true, ClampMin = "0"))
	float OuterCircleRadius;

private:
	virtual void CreateSpawnLocations(TArray<FVector>& OutLocations, const FSpawnData& SpawnData, class UWorld& InWorld) override;
};
