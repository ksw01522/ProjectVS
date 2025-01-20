// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleMap/TitleMapGameModeBase.h"
#include "TitleMap/TitleMapPlayerController.h"

ATitleMapGameModeBase::ATitleMapGameModeBase()
{
	static ConstructorHelpers::FClassFinder<ATitleMapPlayerController> TitleMapControllerClass(TEXT("/Game/TitleMap/BP_TitleMapController.BP_TitleMapController_C"));
	if (TitleMapControllerClass.Succeeded())
	{
		PlayerControllerClass = TitleMapControllerClass.Class;
	}
}
