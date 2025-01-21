// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetStyle/VSRichTextBlockDecorator.h"
#include "Ability/AbilityDataManager.h"

FAbilityDataRichTextDecorator::FAbilityDataRichTextDecorator(URichTextBlock* InOwner)
	: FRichTextDecorator(InOwner)
{
}

void FAbilityDataRichTextDecorator::CreateDecoratorText(const FTextRunInfo& RunInfo, FTextBlockStyle& InOutTextStyle, FString& InOutString) const
{
	UAbilityDataManager* AbilityDataManager = UAbilityDataManager::GetAbilityDataManager();
	if (AbilityDataManager == nullptr) return;
	
	FName DataKey;
	const TMap<FString, FString>& MetaData = RunInfo.MetaData;
	if (MetaData.Contains("Tag"))
	{
		DataKey = FName(MetaData.FindRef("Tag"));
	}

	int Level = 1;
	if (MetaData.Contains("Level"))
	{
		Level = FCString::Atoi(*MetaData.FindRef("Level"));
	}

	bool bResult = false;
	float Value = AbilityDataManager->FindAbilityData(FGameplayTag::RequestGameplayTag(DataKey), bResult, Level);

	ensureMsgf(bResult, TEXT("AbilityData Not Found : %s "), *DataKey.ToString());
	InOutString = FString::SanitizeFloat(Value);
}


UAbilityDataRichTextBlockDecorator::UAbilityDataRichTextBlockDecorator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedPtr<ITextDecorator> UAbilityDataRichTextBlockDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable<ITextDecorator>(new FAbilityDataRichTextDecorator(InOwner));
}


