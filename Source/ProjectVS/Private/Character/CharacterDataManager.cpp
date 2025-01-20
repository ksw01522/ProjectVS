// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CharacterDataManager.h"
#include "ProjectVS.h"
#include "Misc/FileHelper.h"
#include "Engine/DataTable.h"
#include "Character/Monster/VSMonster.h"
#include "GameplayEffectTypes.h"
#include "Player/Upgrade/PlayerUpgradeSubsystem.h"

#include "Engine/AssetManager.h"
#include "Player/PlayableCharacterDataAsset.h"

#if WITH_EDITOR
#define bDebug true
#endif

UCharacterDataManager* UCharacterDataManager::DataManager = nullptr;

const UCharacterDataManager* UCharacterDataManager::GetCharacterDataManager()
{
    return DataManager;
}

void UCharacterDataManager::Initialize(FSubsystemCollectionBase& Collection)
{
    LOG_INFO(TEXT("Start Initialize CharacterDataManager."));
    DataManager = this;

    MonsterDataTable = NewObject<UDataTable>(this);
    MonsterDataTable->RowStruct = FMonsterDataStruct::StaticStruct();

    ReadMonsterDataTableCSV();
    CreateMonsterClassTable();

    StartLoadPlayableCharacterAssets();
    
    LOG_INFO(TEXT("End Initialize CharacterDataManager."));
}

void UCharacterDataManager::Deinitialize()
{
    LOG_INFO(TEXT("Start Deinitialize CharacterDataManager."));
    DataManager = nullptr;

    LOG_INFO(TEXT("End Deinitialize CharacterDataManager."));
}

FString UCharacterDataManager::GetDataTablePath() const
{
    FString ReturnPath = FPaths::ProjectContentDir() + TEXT("DataTable/Monster/MonsterData.csv");

    return ReturnPath;
}

void UCharacterDataManager::ReadMonsterDataTableCSV()
{
    const FString FilePath = GetDataTablePath();

    MonsterDataTable = NewObject<UDataTable>(this);
    MonsterDataTable->RowStruct = FMonsterDataStruct::StaticStruct();
    
    if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
    {
        FString ResultCSV = "";

        FFileHelper::LoadFileToString(ResultCSV, *FilePath);

        MonsterDataTable->CreateTableFromCSVString(ResultCSV);
    }

    const TMap<FName, uint8*> RefMap = MonsterDataTable->GetRowMap();
    for (const auto& E : RefMap)
    {
        FMonsterDataStruct* MonsterData = reinterpret_cast<FMonsterDataStruct*>(E.Value);
#if WITH_EDITOR
        if (bDebug)
        {
            LOG_ERROR(TEXT("%s : HP_Base %1.f, HP_Level %.1f, ATK_Base %.1f, ATK_Level %.1f, MoveSpeed %.1f"), *E.Key.ToString(), MonsterData->HP_Base, MonsterData->HP_Level, MonsterData->ATK_Base, MonsterData->ATK_Level, MonsterData->MoveSpeed);
        }
#endif
    }

}

void UCharacterDataManager::CreateMonsterClassTable()
{
    TArray<FName> KeyNames = MonsterDataTable->GetRowNames();

    const FString RootFolderPath = "/Game/Monsters/";

    for (const auto& KeyName : KeyNames)
    {
        const FString MonsterCode = KeyName.ToString();
        const FString MonsterClassPath = RootFolderPath + MonsterCode + "/" + MonsterCode + "." + MonsterCode + "_C";

        LOG_ERROR(TEXT("%s"), *MonsterClassPath);

        TSubclassOf<AVSMonster> LoadedClass = StaticLoadClass(AVSMonster::StaticClass(), this, *MonsterClassPath);
  
        #if WITH_EDITOR
        if (LoadedClass.GetDefaultObject() == nullptr)
        {
            LOG_ERROR(TEXT("Can't find BP Monster : %s"), *MonsterCode);
            continue;
        }
        #endif
       
        MonsterClassTable.Add(KeyName, LoadedClass);
    }
}

void UCharacterDataManager::StartLoadPlayableCharacterAssets()
{
    LOG_WARNING(TEXT("Start Load Playable Character Asset Data"));

    UAssetManager& AssetManager = UAssetManager::Get();

    TArray<FPrimaryAssetId> Ids;

    FPrimaryAssetType AssetType = UPlayableCharacterDataAsset::GetPlayableCharacterAssetType();

    AssetManager.GetPrimaryAssetIdList(AssetType, Ids);

    AssetManager.LoadPrimaryAssets(Ids, TArray<FName>(), FStreamableDelegate::CreateUObject(this, &UCharacterDataManager::CompleteLoadPlayableCharacterAssets));
}

void UCharacterDataManager::CompleteLoadPlayableCharacterAssets()
{
    UAssetManager& AssetManager = UAssetManager::Get();

    FPrimaryAssetType AssetType = UPlayableCharacterDataAsset::GetPlayableCharacterAssetType();

    TArray<UObject*> AssetObjects;

    if (AssetManager.GetPrimaryAssetObjectList(AssetType, AssetObjects))
    {
        for (const auto& Asset : AssetObjects)
        {
            const UPlayableCharacterDataAsset* DataAsset = Cast<UPlayableCharacterDataAsset>(Asset);

            PlayableCharacterAssetMap.Add(DataAsset->GetCharacterName(), DataAsset);

#if WITH_EDITOR
            if (bDebug)
            {
                LOG_WARNING(TEXT("Loaded {%s} Character"), *DataAsset->GetCharacterName().ToString());
            }
#endif
        }
    }

    LOG_WARNING(TEXT("Complete Load Playable Character Asset Data"));
}

TSubclassOf<AVSMonster> UCharacterDataManager::GetMonsterClass(const FName& InCode) const
{
    if (const TSubclassOf<AVSMonster>* Finded = MonsterClassTable.Find(InCode))
    {
        return *Finded;
    }

    return nullptr;
}

const FMonsterDataStruct* UCharacterDataManager::FindMonsterData(const FName& InCode) const
{
    FString Context = "";
    
    return MonsterDataTable->FindRow<FMonsterDataStruct>(InCode, Context);
}

const UPlayableCharacterDataAsset* UCharacterDataManager::FindPlayableCharacterData(const FName& InName) const
{
    if (const UPlayableCharacterDataAsset* const* FindedData = PlayableCharacterAssetMap.Find(InName))
    {
        return *FindedData;
    }

    LOG_ERROR(TEXT("Can't Find Playable Character Data By : %s"), *InName.ToString());
    return nullptr;
}


#if WITH_EDITOR
#undef bDebug
#endif