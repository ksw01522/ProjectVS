// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectVSPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "ProjectVSCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "ProjectVS.h"
#include "Player/Widget/PlayerMainUIWidget.h"
#include "Player/Ability/Widget/TryAbilityLevelUpWindow.h"
#include "Player/Ability/Widget/AbilityLevelUpWidget.h"
#include "Ability/AbilityBookComponent.h"
#include "VSPlayerState.h"
#include "Components/Widget.h"
#include "Player/WidgetManagerComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AProjectVSPlayerController::AProjectVSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	PrimaryActorTick.bCanEverTick = true;

	WidgetManager = CreateDefaultSubobject<UWidgetManagerComponent>("WidgetManager", true);
}

void AProjectVSPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	InitiailizeWidgets();


	SetInputModeToGame();

	AVSPlayerState* VSPlayerState = GetPlayerState<AVSPlayerState>();
	VSPlayerState->OnLevelUpDelegate.AddDynamic(this, &AProjectVSPlayerController::OnPlayerLevelUp);
}

void AProjectVSPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(DeltaTime == 0) {return;}


	if (APawn* CurrentPawn = GetPawn())
	{
		FHitResult HitResult;
		bool bSucceed = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
		if (bSucceed)
		{
			FVector HitLoc = HitResult.Location;
			HitLoc.Z = 0;

			FVector PawnLocation = CurrentPawn->GetActorLocation();
			PawnLocation.Z = 0;

			FVector Temp = HitLoc - PawnLocation;
			Temp.Normalize();

			FRotator NewRot = Temp.Rotation();
			SetControlRotation(NewRot);
		}
	}
}

void AProjectVSPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 1);
	#if WITH_EDITOR
		Subsystem->AddMappingContext(DebugMappingContext, 0);
	#endif
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
#if WITH_EDITOR
		EnhancedInputComponent->BindAction(ForceLevelUpAbilityAction, ETriggerEvent::Triggered, this, &AProjectVSPlayerController::Debug_ForceLevelUpAbility);
#endif
	}
}

void AProjectVSPlayerController::InitiailizeWidgets()
{
	UPlayerMainUIWidget* NewMainUI = Cast<UPlayerMainUIWidget>(WidgetManager->AddWidget("MainUI", MainUIClass));
	if (NewMainUI)
	{
		NewMainUI->AddToViewport(0);
	}

	if (UTryAbilityLevelUpWindow* NewAbilityLevelUpWindow = CreateWidget<UTryAbilityLevelUpWindow>(this, TryAbilityLevelUpWindowClass, "Ability Level Up Window"))
	{
		WidgetManager->AddWidget("AbilityLevelUpWindow", NewAbilityLevelUpWindow);

		NewAbilityLevelUpWindow->SetVisibility(ESlateVisibility::Collapsed);
		NewAbilityLevelUpWindow->AddToViewport(1);
	}


}

FGenericTeamId AProjectVSPlayerController::GetGenericTeamId() const
{
	AVSPlayerState* VSPlayerState = GetPlayerState<AVSPlayerState>();
	if(VSPlayerState == nullptr) return FGenericTeamId::NoTeam;
	
	return VSPlayerState->GetGenericTeamId();
}



void AProjectVSPlayerController::TryAbilityLevelUp()
{
	if(GetLocalRole() != ROLE_Authority || AbilityLevelUpCount == 0) { return; }
	AbilityLevelUpCount--;

	UAbilityBookComponent* BookComponent = GetPlayerState<AVSPlayerState>()->GetAbilityBookComponent();

	TArray<FAbilityLevelUpTargetInfo> Targets;
	BookComponent->GetLevelUpTargetArray(Targets, 3);

	SelectAbilityLevelUp(Targets);
}

void AProjectVSPlayerController::SelectAbilityLevelUp(const TArray<FAbilityLevelUpTargetInfo>& InTargets)
{
	UTryAbilityLevelUpWindow* AbilityLevelUpWindow = Cast<UTryAbilityLevelUpWindow>(WidgetManager->FindWidget("AbilityLevelUpWindow"));
	check(AbilityLevelUpWindow);

	AbilityLevelUpWindow->SetAbilityLevelUp(InTargets);

	AbilityLevelUpWindow->SetVisibility(ESlateVisibility::Visible);

	SetInputModeToUI(AbilityLevelUpWindow);

	SetPause(true);
}

void AProjectVSPlayerController::SetInputModeToUI(UWidget* FocuseTarget)
{
	FInputModeUIOnly NewInputMode;

	SetShowMouseCursor(true);
	SetInputMode(NewInputMode);

	if (FocuseTarget != nullptr)
	{
		FocuseTarget->SetUserFocus(this);
	}
}

void AProjectVSPlayerController::SetInputModeToGame()
{
	FInputModeGameOnly NewInputMode;
	
	SetShowMouseCursor(true);
		
	SetInputMode(NewInputMode);
}

void AProjectVSPlayerController::OnPlayerLevelUp(int BeforeLevel, int AfterLevel)
{
	int LevelUpCount = AfterLevel - BeforeLevel;

	AddAbilityLevelUpCount(LevelUpCount);
}



#if WITH_EDITOR
void AProjectVSPlayerController::Debug_ForceLevelUpAbility()
{
	AddAbilityLevelUpCount(1);
}
#endif
void AProjectVSPlayerController::AddAbilityLevelUpCount(int InCount)
{
	if (GetLocalRole() != ROLE_Authority) { return; }

	AbilityLevelUpCount += InCount;

	TryAbilityLevelUp();
}

