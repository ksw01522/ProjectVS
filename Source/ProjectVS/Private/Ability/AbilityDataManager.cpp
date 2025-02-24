// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AbilityDataManager.h"
#include "ProjectVS.h"

#include "Ability/VSAbility.h"

#include "Engine/AssetManager.h"

#if WITH_EDITOR
	#define MUST_DEBUG true
#endif

UAbilityDataManager* UAbilityDataManager::ADMInstance = nullptr;

UAbilityDataManager* UAbilityDataManager::GetAbilityDataManager()
{

	return ADMInstance;
}

void UAbilityDataManager::StartLoadAbilities()
{
	UAssetManager& AssetManager = UAssetManager::Get();

	FPrimaryAssetType AssetType = UVSAbility::GetAbilityAssetType();
	TArray<FPrimaryAssetId> Ids;
	AssetManager.GetPrimaryAssetIdList(AssetType, Ids);

	AssetManager.LoadPrimaryAssets(Ids, {}, FStreamableDelegate::CreateUObject(this, &UAbilityDataManager::CompleteLoadAbilities));
}

void UAbilityDataManager::CompleteLoadAbilities()
{
	UAssetManager& AssetManager = UAssetManager::Get();

	FPrimaryAssetType AssetType = UVSAbility::GetAbilityAssetType();
	TArray<UObject*> LoadedAssets;

	AssetManager.GetPrimaryAssetObjectList(AssetType, LoadedAssets);

	for (auto& LoadedObj : LoadedAssets)
	{
		UClass* LoadedClass = Cast<UClass>(LoadedObj);
		UVSAbility* LoadedAbility = LoadedClass->GetDefaultObject<UVSAbility>();

		FName TempCode = LoadedAbility->GetAbilityCode();

		AbilityArray.Add(TempCode, LoadedClass);
	}

#if WITH_EDITOR
	if(MUST_DEBUG) LOG_ERROR(TEXT("LoadedAbility Count = %d"), AbilityArray.Num());
#endif
}

TSubclassOf<UVSAbility> UAbilityDataManager::FindAbility(const FName& Code) const
{
	if(!AbilityArray.Contains(Code)) return nullptr;

	return AbilityArray.FindRef(Code);
}

void UAbilityDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
	LOG_INFO(TEXT("Start Initalize Ability Data Manager."));

	ADMInstance = this;

	ReadAbilityDataCSV();
	StartLoadAbilities();

	LOG_INFO(TEXT("End Initalize Ability Data Manager."));
}

void UAbilityDataManager::Deinitialize()
{
	LOG_INFO(TEXT("Start Deinitalize Ability Data Manager."));

	ADMInstance = nullptr;	

	DataMap.Empty();
	LOG_INFO(TEXT("End Deinitalize Ability Data Manager."));

}

float UAbilityDataManager::FindAbilityData(const FGameplayTag& DataTag, bool& bResult, int Level) const
{
	bResult = false;

	if(Level <= 0) {
		LOG_ERROR(TEXT("Try Find Ability Data By Level : %d"), Level);
		return 0;
	}

	FName TagName = DataTag.GetTagName();
	const TArray<float>* FindedArray = DataMap.Find(TagName);
	if (FindedArray == nullptr)
	{
		LOG_ERROR(TEXT("Can't Find AbilityData : %s"), *DataTag.GetTagName().ToString());
		return 0;
	}

	const TArray<float>& RefArray = *FindedArray;

	if (RefArray.Num() <= Level)
	{
		LOG_ERROR(TEXT("Try Find {%s} Level : %d, in : %d"), *DataTag.GetTagName().ToString(), Level, FindedArray->Num());
		return 0;
	}

	bResult = true;

#if WITH_EDITOR
	if(MUST_DEBUG) LOG_INFO(TEXT("Finded {%s}, Level : %d, Value : %.1f"), *DataTag.GetTagName().ToString(), Level, RefArray[Level]);
#endif
	return RefArray[Level];
}

bool UAbilityDataManager::InitializeAddableAbilities(const FName& InCharacterName, TArray<FName>& OutAbilities)
{
	//임시로 모든 어빌리티 집어넣기
	OutAbilities.Reserve(OutAbilities.Num() + AbilityArray.Num());

	for (const auto& Ability : AbilityArray)
	{
		OutAbilities.Add(Ability.Key);
	}

	return true;
}

void UAbilityDataManager::ReadAbilityDataCSV()
{
	const FString CSVFolderPath = FPaths::ProjectContentDir() + TEXT("DataTable/Ability/");

	TArray<FString> DataFileNames;

	IFileManager::Get().FindFiles(DataFileNames, *CSVFolderPath, TEXT("*.csv"));

	//LOG_ERROR(TEXT("%d"), DataFileNames.Num());

	//for (const auto& FileName : DataFileNames)
	//{
	//	LOG_ERROR(TEXT("%s"), *FileName);
	//}

	FString LoadResult;
	FString DataFilePath;
	FString DataCategory;

	for (const auto& DataFileName : DataFileNames)
	{
		DataFilePath = CSVFolderPath + DataFileName;
		
		DataCategory = DataFileName.Left(DataFileName.Len() - 4);
		
		#if WITH_EDITOR
		if(MUST_DEBUG) LOG_ERROR(TEXT("DataCategory = %s"), *DataCategory);
		#endif
		FFileHelper::LoadFileToString(LoadResult, *DataFilePath);

		TArray<FString> ParseLines;

		LoadResult.ParseIntoArray(ParseLines, TEXT("\n"));

		int LinesNum = ParseLines.Num();
		for (int i = 1; i < LinesNum; i++)
		{
			TArray<FString> AbilityData;

			ParseLines[i].ParseIntoArray(AbilityData, TEXT(","));
			if(AbilityData[0] == "") {continue;}

			int DataNum = AbilityData.Num();

			FName TempMapKey = FName(DataCategory + "." + AbilityData[0]);
			DataMap.FindOrAdd(TempMapKey).Add(0);

			for (int j = 1; j < DataNum; j++)
			{
				DataMap.FindOrAdd(TempMapKey).Add(FCString::Atof(*AbilityData[j]));
			}
		}
	}



}

#undef MUST_DEBUG