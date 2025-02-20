// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityEditorSubsystem.h"
#include "Engine/CurveTable.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "AbilityTesterEditor.h"
#include "GameplayAbility.h"
#include "Engine/AssetManager.h"
#include "Ability/VSAbility.h"


TObjectPtr<UAbilityEditorSubsystem> UAbilityEditorSubsystem::StaticAESInstance = nullptr;

UAbilityEditorSubsystem::UAbilityEditorSubsystem()
{
	
}

void UAbilityEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	StaticAESInstance = this;

	RootFolderPath = FPaths::ProjectContentDir() + TEXT("DataTable/Ability/");

	ReadAbilityDataCSV();
}

void UAbilityEditorSubsystem::Deinitialize()
{
	StaticAESInstance = nullptr;
}

UAbilityEditorSubsystem* UAbilityEditorSubsystem::GetAbilityEditorSubsystem()
{
	return StaticAESInstance;
}

void UAbilityEditorSubsystem::ReadAbilityDataCSV()
{
	DataMap.Empty();

	TArray<FString> DataFileNames;

	IFileManager::Get().FindFiles(DataFileNames, *RootFolderPath, TEXT("*.csv"));

	for (auto DataFileName : DataFileNames)
	{
		FString LoadResult = "";

		FString FilePath = RootFolderPath + DataFileName;

		FString DataCategory = DataFileName.Left(DataFileName.Len() - 4);


		if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
		{
			LOG_ERROR(TEXT("Not Exist Ability Data File : %s"), *FilePath);
			continue;
		}

		FFileHelper::LoadFileToString(LoadResult, *FilePath);

		TArray<FString> ParseLines;

		LoadResult.ParseIntoArray(ParseLines, TEXT("\n"));

		for (int i = 1; i < ParseLines.Num(); i++)
		{
			TArray<FString> AbilityData;

			ParseLines[i].ParseIntoArray(AbilityData, TEXT(","));
			
			FName KeyName = FName(AbilityData[0]);
			TArray<float> ValueArray;	
			ValueArray.Add(0);

			for (int j = 1; j < AbilityData.Num(); j++)
			{
				ValueArray.Add(FCString::Atof(*AbilityData[j]));
			}
			
			FName TempKey = FName(DataCategory);

			TMap<FName, TArray<float>>& FindedMap = DataMap.FindOrAdd(TempKey);
			if (FindedMap.Find(KeyName) != nullptr)
			{
				LOG_ERROR(TEXT("Already Read Data {%s} from {%s}"), *AbilityData[0], *DataFileName);
				continue;
			}

			FindedMap.FindOrAdd(FName(DataCategory + "." + AbilityData[0])) = ValueArray;
		}
	}
}

void UAbilityEditorSubsystem::WriteAbilityDataCSV() const
{
	for (const auto& DMaps : DataMap)
	{
		FString NewTableString = "Type,Value";

		for (const auto& DataArray : DMaps.Value)
		{
			NewTableString += '\n';									//줄바꿈
			NewTableString += DataArray.Key.ToString();					//타입
			
			for (const auto& Data : DataArray.Value)
			{
				NewTableString += ',';									//CSV 구문
				NewTableString += FString::SanitizeFloat(Data);	//값
			}
		}

		FString NewFilePath = RootFolderPath + DMaps.Key.ToString() + ".csv";
		bool SaveResult =FFileHelper::SaveStringToFile(NewTableString, *NewFilePath);

		if (SaveResult)
		{
			LOG_WARNING(TEXT("Succedd Save To %s"), *NewFilePath);
		}
		else
		{
			LOG_ERROR(TEXT("Fail Save To %s"), *NewFilePath);
		}
	}
}

void UAbilityEditorSubsystem::ReReadAbilityDataCSV()
{
	ReadAbilityDataCSV();
}

void UAbilityEditorSubsystem::StartLoadAbilitiesForApplyData() const
{
	UAssetManager& AssetManager = UAssetManager::Get();

	FPrimaryAssetType AssetType = UVSAbility::GetAbilityAssetType();
	TArray<FPrimaryAssetId> Ids;
	AssetManager.GetPrimaryAssetIdList(AssetType, Ids);

	AssetManager.LoadPrimaryAssets(Ids, {}, FStreamableDelegate::CreateUObject(this, &UAbilityEditorSubsystem::CompleteLoadAbilitiesForAplyData));

}

void UAbilityEditorSubsystem::CompleteLoadAbilitiesForAplyData() const
{
	UAssetManager& AssetManager = UAssetManager::Get();

	FPrimaryAssetType AssetType = UVSAbility::GetAbilityAssetType();
	TArray<UObject*> LoadedAssets;

	AssetManager.GetPrimaryAssetObjectList(AssetType, LoadedAssets);

	for (auto& LoadedObj : LoadedAssets)
	{
		UVSAbility* LoadedAbility = StaticCast<UVSAbility*>(LoadedObj);

		LoadedAbility->LoadDataFromDataManager();
	}
}




void UAbilityEditorSubsystem::AddAbilityData(const TMap<FName, TArray<float>>& NewData)
{
	for (const auto& ND : NewData)
	{
		FName TagName = ND.Key;
		if(TagName.IsNone()) continue;

		FString TagString = TagName.ToString();

		//CSV파일 이름
		int dotIdx = 0;
		TagString.FindChar('.', dotIdx);
		FString FirstTagString = TagString.Left(dotIdx);
		FName FirstTagName(FirstTagString);
		if(FirstTagName.IsNone()) {FirstTagName = TagName;}

		TMap<FName, TArray<float>>& FindedMap = DataMap.FindOrAdd(FirstTagName);
		
		FindedMap.FindOrAdd(TagName) = ND.Value;
	}

	WriteAbilityDataCSV();
}


float UAbilityEditorSubsystem::FindAbilityData(const FGameplayTag& InTag, bool& bFinded, int Level) const
{
	FName TagName = InTag.GetTagName();
	FString TagString = TagName.ToString();

	//CSV파일 이름
	int dotIdx = 0;
	TagString.FindChar('.', dotIdx);
	FString FirstTagString = TagString.Left(dotIdx);
	FName FirstTagName(FirstTagString);

	const TMap<FName, TArray<float>>* FindedMap = DataMap.Find(FirstTagName);
	if (FindedMap == nullptr)
	{
		bFinded = false;
		return 0;
	}

	const TArray<float>* FindedArray = FindedMap->Find(TagName);
	
	if (FindedArray == nullptr)
	{
		bFinded = false;
		return 0;
	}
	
	if (FindedArray->Num() <= Level)
	{
		bFinded = false;
		return 0;
	}

	return (*FindedArray)[Level];
}

bool UAbilityEditorSubsystem::FindAbilityDataArray(TArray<float>& Out, const FGameplayTag& InTag) const
{
	FName TagName = InTag.GetTagName();
	FString TagString = TagName.ToString();

	//CSV파일 이름
	int dotIdx = 0;
	TagString.FindChar('.', dotIdx);
	FString FirstTagString = TagString.Left(dotIdx);
	FName FirstTagName(FirstTagString);

	if(FirstTagName.IsNone()) FirstTagName = TagName;

	const TMap<FName, TArray<float>>* FindedMap = DataMap.Find(FirstTagName);
	if(FindedMap == nullptr) return false;

	const TArray<float>* FindedArray = FindedMap->Find(TagName);
	if(FindedArray == nullptr) return false;

	Out = *FindedArray;
	return true;
}


