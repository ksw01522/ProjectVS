// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "PaperVSCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVS_API APaperVSCharacter : public APaperZDCharacter, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:
	APaperVSCharacter();

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> SpritePivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bLookRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UWidgetComponent> HPBarWidget;

//AbilitySystem
private:
	TWeakObjectPtr<class UAbilitySystemComponent> WeakASC;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	void RegisterAbilitySystem(UAbilitySystemComponent* NewASC);
	void UnregisterAbilitySystem();

	USceneComponent* GetSpritePivot() const {return SpritePivot;}

//Move Speed
private:
	FDelegateHandle OnMoveSpeedChangedHandle;

	void OnMoveSpeedChanged(const struct FOnAttributeChangeData& InData);

protected:
	virtual void SetMoveSpeed(float NewSpeed);

private:
	FDelegateHandle OnChangedHPHandle;
	FDelegateHandle OnChangedMaxHPHandle;

	virtual void OnChangedHP(const struct FOnAttributeChangeData& InData);
	virtual void OnChangedMaxHP(const struct FOnAttributeChangeData& InData);


public:
	virtual FGenericTeamId GetGenericTeamId() const override {return FGenericTeamId::NoTeam;}

	void SetLookRight(bool NewState);

	void UpdateHPBarWidget();

};
