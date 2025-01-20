// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTesterActor.h"
#include "AbilitySystemComponent.h"
#include "Ability/AttributeSet_Default.h"
#include "Ability/AttributeSet_Player.h"

// Sets default values
AAbilityTesterActor::AAbilityTesterActor()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystem");
	ATS_Default = CreateDefaultSubobject<UAttributeSet_Default>("ATS_Default");
	ATS_Player = CreateDefaultSubobject<UAttributeSet_Player>("ATS_Player");

	AbilitySystem->SetSpawnedAttributes({ATS_Default, ATS_Player});
}

// Called when the game starts or when spawned
void AAbilityTesterActor::BeginPlay()
{
	Super::BeginPlay();
	
	ATS_Default->SetMaxHP(MaxHP);
	ATS_Player->SetDamageRate(DamageRate);
	ATS_Player->SetCoolTimeRate(CoolTimeRate);
	ATS_Player->SetProjectileSpeed(ProjectileSpeed);
	ATS_Player->SetProjectileCount(ProjectileCount);
	ATS_Player->SetEffectTimeRate(EffectTimeRate);
	ATS_Player->SetEffectRangeRate(EffectRangeRate);
}

// Called every frame
void AAbilityTesterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAbilityTesterActor::SetTestAbility(TSubclassOf<UGameplayAbility> InAbility, int TestLevel)
{
	if (TestAbility.GetDefaultObject())
	{
		AbilitySystem->ClearAbility(TestAbilitySpecHandle);
	}

	TestAbility = InAbility;
	if (TestAbility.GetDefaultObject())
	{
		TestAbilitySpecHandle = AbilitySystem->GiveAbility(FGameplayAbilitySpec(TestAbility, TestLevel));
	}
	else
	{
		TestAbilitySpecHandle = FGameplayAbilitySpecHandle();
	}
}

bool AAbilityTesterActor::TryActivateTestAbility() const
{
	return AbilitySystem->TryActivateAbility(TestAbilitySpecHandle);
}

