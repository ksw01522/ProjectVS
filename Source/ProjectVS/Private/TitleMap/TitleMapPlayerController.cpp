// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleMap/TitleMapPlayerController.h"
#include "TitleMap/TitleMapUIWidget.h"
#include "TitleMap/UpgradeWindowWidget.h"

void ATitleMapPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UIWidget = CreateWidget<UTitleMapUIWidget>(this, UIWidgetClass, "TitleMapUIWidget");
	UIWidget->AddToViewport();

	InputComponent->bBlockInput = true;

	SetShowMouseCursor(true);

	FInputModeUIOnly UIInputMode;

	SetInputMode(UIInputMode);

	UIWidget->GetUpgradeWindow()->SetFocus();
}
