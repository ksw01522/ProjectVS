// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet_Default.generated.h"

#define ATTRIBUTE_CATEGORY "Default"

#define GAMEPLAYATTRIBUTE_TAG_GETTER(PropertyName) \
	FORCEINLINE static FGameplayTag Get##PropertyName##Tag() \
	{ \
		FString Category = ATTRIBUTE_CATEGORY; \
		FString PName = #PropertyName; \
		return FGameplayTag::RequestGameplayTag(FName(FString::Printf(TEXT("Attribute.%s.%s"), *Category, *PName) ) ); \
	}

#define GAMEPLAYATTRIBUTE_NAME_GETTER(PropertyName) \
FORCEINLINE static FName Get##PropertyName##Name() \
{\
	return FName(#PropertyName);\
}

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName) \
	GAMEPLAYATTRIBUTE_NAME_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_TAG_GETTER(PropertyName)
/**
 * 
 */
UCLASS()
class PROJECTVS_API UAttributeSet_Default : public UAttributeSet
{
	GENERATED_BODY()
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	//virtual void PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const override;

	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float OldMaxValue, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);


private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Level, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Level;

	UFUNCTION()
	void OnRep_Level(const FGameplayAttributeData& OldLevel);

public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Default, Level);


private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HP, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData HP;

	UFUNCTION()
	void OnRep_HP(const FGameplayAttributeData& OldHP);

public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Default, HP);



private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHP, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MaxHP;

	UFUNCTION()
	void OnRep_MaxHP(const FGameplayAttributeData& OldMaxHP);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Default, MaxHP);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MoveSpeed, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData MoveSpeed;

	UFUNCTION()
	void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Default, MoveSpeed);

private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FGameplayAttributeData Damage;
public:
	ATTRIBUTE_ACCESSORS(UAttributeSet_Default, Damage);

private:
	void MaxOverHP(float Delta);
};

#undef ATTRIBUTE_CATEGORY