// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Ability/Widget/AbilityLevelUpWidget.h"
#include "Ability/AbilityDataManager.h"
#include "Ability/VSAbility.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "VSLocalPlayer.h"
#include "Ability/AbilityBookComponent.h"
#include "ProjectVSPlayerController.h"
#include "Framework/Application/SlateApplication.h"
#include "SlateOptMacros.h"
#include "Types/NavigationMetaData.h"
#include "ProjectVS.h"
#include "VSPlayerState.h"
#include "Components/RichTextBlock.h"
#include "Player/VSPlayerState.h"

UAbilityLevelUpWidget::UAbilityLevelUpWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsFocusable(true);
}

void UAbilityLevelUpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateAbilityLevelUpWidget();
}

void UAbilityLevelUpWidget::NativeDestruct()
{

	Super::NativeDestruct();
}

FReply UAbilityLevelUpWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FSlateApplication& SlateApp = FSlateApplication::Get();

	if (EUINavigationAction Action = SlateApp.GetNavigationActionFromKey(InKeyEvent); Action == EUINavigationAction::Accept)
	{
		OnSelectedLevelUp(GetOwningPlayer<AProjectVSPlayerController>());
		return FReply::Handled();
	}
	
	if (EUINavigation NavDir = SlateApp.GetNavigationDirectionFromKey(InKeyEvent); NavDir != EUINavigation::Invalid && NavDir != EUINavigation::Num)
	{
		const ENavigationGenesis Genesis = InKeyEvent.GetKey().IsGamepadKey() ? ENavigationGenesis::Controller : ENavigationGenesis::Keyboard;
		return FReply::Handled().SetNavigation(NavDir, Genesis);
	}

	return FReply::Unhandled();
}

FReply UAbilityLevelUpWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		OnSelectedLevelUp(GetOwningPlayer<AProjectVSPlayerController>());

	}


	return FReply::Handled();
}

void UAbilityLevelUpWidget::UpdateAbilityLevelUpWidget()
{
	const UAbilityDataManager* ADM = UAbilityDataManager::GetAbilityDataManager();
	if(ADM == nullptr) return;

	TSubclassOf<UVSAbility> AbilityClass = ADM->FindAbility(AbilityCode);
		
	const UVSAbility* AbilityCDO = StaticCast<UVSAbility*>(AbilityClass.GetDefaultObject());

	AVSPlayerState* VSPlayerState = GetOwningPlayerState<AVSPlayerState>();
	check(VSPlayerState)

	UAbilityBookComponent* AbilityBook = VSPlayerState->GetAbilityBookComponent();

	if (AbilityCDO != nullptr)
	{
		SetIcon(AbilityCDO->GetAbilityIcon());
		SetNameText(AbilityCDO->GetAbilityName());
		SetDescriptionText(AbilityCDO->GetLeveUpDescriptionText(AbilityBook, CurrentLevel, TargetLevel));
		SetLevelText(TargetLevel);
	}
	else
	{
		SetIcon(nullptr);
		SetNameText(FText::GetEmpty());
		SetDescriptionText(FText::GetEmpty());
		SetLevelText(0);
	}
}

void UAbilityLevelUpWidget::SetIcon(UTexture2D* InTexture)
{
	if(IconImage == nullptr) return;

	IconImage->SetBrushFromTexture(InTexture);
}

void UAbilityLevelUpWidget::SetNameText(const FText& InText)
{
	if(NameText == nullptr) return;

	NameText->SetText(InText);
}

void UAbilityLevelUpWidget::SetDescriptionText(const FText& InText)
{
	if(DescriptionText == nullptr) return;

	DescriptionText->SetText(InText);
}

void UAbilityLevelUpWidget::SetLevelText(float InLevel)
{
	if(LevelText == nullptr) return;

	FFormatNamedArguments Args;
	Args.Add("Level", InLevel);

	LevelText->SetText(FText::Format(NSLOCTEXT("AbilityLevelUpWidget", "Level", "Lv. {Level}"), Args));
}

void UAbilityLevelUpWidget::SetAbilityLevelUpInfo(const FAbilityLevelUpTargetInfo& InInfo)
{
	AbilityCode = InInfo.Code;
	CurrentLevel = InInfo.CurrentLevel;
	TargetLevel = InInfo.TargetLevel;

	UpdateAbilityLevelUpWidget();

}

void UAbilityLevelUpWidget::OnSelectedLevelUp(AProjectVSPlayerController* PlayerController)
{
	if(PlayerController == nullptr) return;
	
	UAbilityBookComponent* BookSystem = PlayerController->GetPlayerState<AVSPlayerState>()->GetAbilityBookComponent();

	BookSystem->SetOrAddPageByLevelUp(AbilityCode, TargetLevel);

	if (OnSelectedLevelUpDelegate.IsBound())
	{
		OnSelectedLevelUpDelegate.Broadcast();
	}
}
