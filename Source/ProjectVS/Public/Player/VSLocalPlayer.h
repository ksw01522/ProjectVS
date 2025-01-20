// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "VSLocalPlayer.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTVS_API UVSLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()

private:
	FName CharacterName;



public:
	FName GetSelectedCharacterName() const { return CharacterName; }
	void SetSelectedCharacterName(const FName& InCharacterName) { CharacterName = InCharacterName; }



};
