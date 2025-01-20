// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PaperVSCharacter.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "VSMonster.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMonsterDeadDelegate, class AVSMonster*)

UCLASS()
class PROJECTVS_API AVSMonster : public APaperVSCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVSMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAbilitySystemComponent> AbilitySystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAttributeSet_Default> ATS_Default;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAttributeSet_Monster> ATS_Monster;

	FOnMonsterDeadDelegate OnMonsterDead;

private:
	void InitializeAbilitySystem();

	void OnHPChanged(const struct FOnAttributeChangeData& InData);

	void OnDeadTagAdded(struct FGameplayTag InTag, int NewCount);

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	
	virtual FGenericTeamId GetGenericTeamId() const override;

	FOnMonsterDeadDelegate& GetOnMonsterDeadDelegate() {return OnMonsterDead;}

	float GetBountyEXP() const;
	float GetBountyGold() const;
};
