// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Ability/Widget/TryAbilityLevelUpWindow.h"
#include "Player/Ability/Widget/AbilityLevelUpWidget.h"
#include "Components/VerticalBox.h"
#include "ProjectVSPlayerController.h"
#include "Ability/AbilityBookComponent.h"

UTryAbilityLevelUpWindow::UTryAbilityLevelUpWindow(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsFocusable(true);
}

void UTryAbilityLevelUpWindow::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UWidget*> Children = LevelUpWidgetBox->GetAllChildren();

	for (auto& Child : Children)
	{
		if (UAbilityLevelUpWidget* TempLevelUpWidget = Cast<UAbilityLevelUpWidget>(Child))
		{
			LevelUpWidgets.Add(TempLevelUpWidget);
			TempLevelUpWidget->OnSelectedLevelUpDelegate.AddUObject(this, &UTryAbilityLevelUpWindow::OnSelectedLevelUpWidget);
		}
	}
}


void UTryAbilityLevelUpWindow::OnSelectedLevelUpWidget()
{
	AProjectVSPlayerController* VSPC = GetOwningPlayer<AProjectVSPlayerController>();

	SetVisibility(ESlateVisibility::Collapsed);
	HiddenLevelUpWidgets();
	
	VSPC->SetPause(false);
	VSPC->SetInputModeToGame();

	VSPC->TryAbilityLevelUp();
}

void UTryAbilityLevelUpWindow::AddLevelUpWidget(UAbilityLevelUpWidget* Target)
{
	if(Target == nullptr) return;

	LevelUpWidgetBox->AddChildToVerticalBox(Target);
}

void UTryAbilityLevelUpWindow::ClearLevelUpWidgetBox()
{
	LevelUpWidgetBox->ClearChildren();
}

void UTryAbilityLevelUpWindow::HiddenLevelUpWidgets()
{
	for (auto& LevelUpWidget : LevelUpWidgets)
	{
		LevelUpWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UTryAbilityLevelUpWindow::SetAbilityLevelUp(const TArray<FAbilityLevelUpTargetInfo>& Targets)
{
	int Size = Targets.Num();

	HiddenLevelUpWidgets();

	for (int i = 0; i < Size; i++)
	{
		LevelUpWidgets[i]->SetAbilityLevelUpInfo(Targets[i]);
		LevelUpWidgets[i]->SetVisibility(ESlateVisibility::Visible);
	}
}
