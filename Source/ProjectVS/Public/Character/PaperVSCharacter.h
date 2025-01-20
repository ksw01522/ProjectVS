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

private:
private:
	UPROPERTY(VisibleAnywhere, Category = "Character", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> SpritePivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	bool bLookRight;


//AbilitySystem
private:
	TWeakObjectPtr<class UAbilitySystemComponent> WeakASC;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	void RegisterAbilitySystem(UAbilitySystemComponent* NewASC);
	void UnregisterAbilitySystem();

//Move Speed
private:
	FDelegateHandle OnMoveSpeedChangedHandle;

	void OnMoveSpeedChanged(const struct FOnAttributeChangeData& InData);

protected:
	virtual void SetMoveSpeed(float NewSpeed);

public:
	virtual FGenericTeamId GetGenericTeamId() const override {return FGenericTeamId::NoTeam;}

	void SetLookRight(bool NewState);


};
