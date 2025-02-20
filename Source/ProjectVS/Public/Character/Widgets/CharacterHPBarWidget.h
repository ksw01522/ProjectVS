// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVS_API UCharacterHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HPProgressBar;

public:
	UProgressBar* GetHPProgressBar() const { return HPProgressBar; }
};
