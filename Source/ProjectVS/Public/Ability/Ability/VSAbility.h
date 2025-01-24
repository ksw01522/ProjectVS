// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AssetRegistry/AssetBundleData.h"
#include "VSAbility.generated.h"


UENUM()
enum class EVSAbilityType : uint8
{
	None = 0 UMETA(Hidden),
	Active = 1,
	Passive = 2,
};

UCLASS(Abstract)
class PROJECTVS_API UVSAbility : public UGameplayAbility
{
	GENERATED_BODY()

	friend class UAbilityArray;

public:
	UVSAbility(const FObjectInitializer& ObjectInitializer);

	
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	static FPrimaryAssetType GetAbilityAssetType();

	virtual void PostLoad() override;

protected:
#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData();

	/** Updates AssetBundleData */
	virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;
protected:
	/** Asset Bundle data computed at save time. In cooked builds this is accessible from AssetRegistry */
	UPROPERTY()
	FAssetBundleData AssetBundleData;
#endif

private:

	UPROPERTY(EditDefaultsOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	FName AbilityCode;

	UPROPERTY(EditDefaultsOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	FName AbilityCategory;

	UPROPERTY(EditDefaultsOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	FText AbilityName;
	

	UPROPERTY(EditDefaultsOnly, Category = "Ability", meta = (AllowPrivateAccess = "true", Categories = "MaxLevel"))
	FGameplayTag MaxLevelTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient, EditDefaultsOnly, Category = "Ability", meta = (AllowPrivateAccess = "true", ClampMin = "1"))
	float MaxLevel;
#endif
	UPROPERTY(EditDefaultsOnly, Category = "Icon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UTexture2D> AbilityIcon;

	UPROPERTY(VisibleDefaultsOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	EVSAbilityType AbilityType;

	UPROPERTY(EditDefaultsOnly, Category = "Cooldowns", meta = (AllowPrivateAccess = "true", Categories = "CoolTime"))
	FGameplayTag CoolTimeTag;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient, EditAnywhere, Category = "Cooldowns", meta = (AllowPrivateAccess = "true"))
	TArray<float> CoolTime;
#endif

protected:
	UPROPERTY(Transient)
	mutable FGameplayTagContainer CachedCooldownTags;


protected:
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	void SetAbilityType(EVSAbilityType NewType) {AbilityType = NewType;}
	void SetAbilityCategory(const FName& NewCategory) {AbilityCategory = NewCategory;}
#if WITH_EDITOR
protected:
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	UFUNCTION(CallInEditor, Category = "AbilityEditor")
	void LoadDataFromDataManager();

protected:
	virtual void NativeLoadDataFromDataManager();

	void ApplyDataFromDataManager(const FGameplayTag& InTag, float& InData);
	void ApplyDataFromDataManager(const FGameplayTag& InTag, TArray<float>& InData);
#endif

	

public:
	FText GetAbilityName() const { return AbilityName; }

	FName GetAbilityCode() const {return AbilityCode;}

	float GetMaxLevel() const;

	UFUNCTION(BlueprintPure, Category = Ability)
	EVSAbilityType GetAbilityType() const {return AbilityType;}
	
	UFUNCTION(BlueprintPure)
	virtual bool IsEvolutionAbility() const { return false; }

	virtual bool CanEnterAbilityBook(class UAbilityBookComponent* InBook) const { return true; }

#if WITH_EDITOR
public:
	virtual void CreateAbilityDataToSave(TMap<FName, TArray<float>>& InMap) const;
protected:
	virtual void PostCDOCompiled(const FPostCDOCompiledContext& Context) override;
	virtual EDataValidationResult IsDataValid(class FDataValidationContext& Context) const override;

#endif

private:
	float GetCooldownTime(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo) const;

protected:
	void SetCooldownTag(const FGameplayTag& NewTag);

public:
	FGameplayTag GetCooldownTag() const {return CoolTimeTag;}

	virtual const FGameplayTagContainer* GetCooldownTags() const override;
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	virtual bool CanSetOrAddInBook(const class UAbilityBookComponent& InBook, int NewLevel) const;

public:
	UTexture2D* GetAbilityIcon() const { return AbilityIcon; }

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FText GetDescriptionText() const;

	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	FText GetLeveUpDescriptionText(int BeforeLevel, int AfterLevel) const;

protected:
	virtual FText GetDescriptionText_Implementation() const;

	virtual FText GetLeveUpDescriptionText_Implementation(int BeforeLevel, int AfterLevel) const;

};
