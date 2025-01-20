// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "GenericTeamAgentInterface.h"
#include "ProjectVSPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS()
class AProjectVSPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AProjectVSPlayerController();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

protected:

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetManagerComponent> WidgetManager;

public:
	UWidgetManagerComponent* GetWidgetManager() const { return WidgetManager; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPlayerMainUIWidget> MainUIClass;

	void InitiailizeWidgets();

public:
	virtual FGenericTeamId GetGenericTeamId() const override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UTryAbilityLevelUpWindow> TryAbilityLevelUpWindowClass;

	int AbilityLevelUpCount = 0;

public:
	void AddAbilityLevelUpCount(int InCount);

	UFUNCTION()
	void TryAbilityLevelUp();

	void SelectAbilityLevelUp(const TArray<struct FAbilityLevelUpTargetInfo>& InTargets);

	void SetInputModeToUI(class UWidget* FocuseTarget = nullptr);
	void SetInputModeToGame();

private:
	UFUNCTION()
	void OnPlayerLevelUp(int BeforeLevel, int AfterLevel);



#if WITH_EDITORONLY_DATA
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DebugMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Debug", meta = (AllowPrivateAccess))
	TObjectPtr<class UInputAction> ForceLevelUpAbilityAction;

	UFUNCTION()
	void Debug_ForceLevelUpAbility();

#endif WITH_EDITORONLY_DATA
};


