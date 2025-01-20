// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitleMapPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVS_API ATitleMapPlayerController : public APlayerController
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTitleMapUIWidget> UIWidgetClass;

	UPROPERTY()
	TObjectPtr<UTitleMapUIWidget> UIWidget;



public:
	
};
