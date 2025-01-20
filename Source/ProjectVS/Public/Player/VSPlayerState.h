// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "VSPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelUpDelegate, int, BeforeLevel, int, AfterLevel);

/**
 * 
 */
UCLASS()
class PROJECTVS_API AVSPlayerState : public APlayerState, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	AVSPlayerState();

protected:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


private:
	UPROPERTY(Replicated)
	FName CharacterName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category  = "Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAbilitySystemComponent> ASC;

	UPROPERTY()
	TObjectPtr<class UAttributeSet_Default> ATS_Default;

	UPROPERTY()
	TObjectPtr<class UAttributeSet_Player> ATS_Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAbilityBookComponent> AbilityBook;

public:
	FOnLevelUpDelegate OnLevelUpDelegate;


private:
	void SetCharacterName(const FName& NewName);

	void InitializeASC();


public:
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return ASC;}
	UAbilityBookComponent* GetAbilityBookComponent() const {return AbilityBook;}

private:
	FTimerHandle CheckLevelUpTimerHandle;

public:
	void GainEXP(float InEXP);

	void CheckLevelUp();


public:
	void GainGold(float InGold);

	UFUNCTION(Client, Reliable)
	void SaveGold();
	void SaveGold_Implementation();

//////////////////Generic TeamID Agent Interface Override
private:
	FGenericTeamId TeamID;

public:
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override {TeamID = NewTeamID;}

	virtual FGenericTeamId GetGenericTeamId() const override { return TeamID; }
};
