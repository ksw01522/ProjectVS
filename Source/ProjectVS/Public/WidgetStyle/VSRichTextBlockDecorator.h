// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "VSRichTextBlockDecorator.generated.h"

class FAbilityDataRichTextDecorator : public FRichTextDecorator
{
public:
	FAbilityDataRichTextDecorator(URichTextBlock* InOwner);

	virtual ~FAbilityDataRichTextDecorator() {}

	/** Override this function to specify which types of tags are handled by this decorator */
	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
		return RunParseResult.Name == TEXT("AbilityData");
	}

protected:
	/** Override this function if you want to dynamically generate text, optionally changing the style. InOutString will start as the content between tags */
	virtual void CreateDecoratorText(const FTextRunInfo& RunInfo, FTextBlockStyle& InOutTextStyle, FString& InOutString) const;
};
/**
 * 
 */
UCLASS(DisplayName = "AbilityData Deco")
class PROJECTVS_API UAbilityDataRichTextBlockDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()
public:
	UAbilityDataRichTextBlockDecorator(const FObjectInitializer& ObjectInitializer);

	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner);
};
