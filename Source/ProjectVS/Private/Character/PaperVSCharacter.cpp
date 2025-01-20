// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PaperVSCharacter.h"
#include "PaperFlipbookComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Ability/AttributeSet_Default.h"

APaperVSCharacter::APaperVSCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;	
	

	GetCharacterMovement()->bOrientRotationToMovement = false; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->SetPlaneConstraintEnabled(true);
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Z);
	GetCharacterMovement()->bSnapToPlaneAtStart = false;

	SpritePivot = CreateDefaultSubobject<USceneComponent>(TEXT("SpritePivot"));
	SpritePivot->SetupAttachment(GetRootComponent());
	SpritePivot->SetUsingAbsoluteRotation(true);
	
	GetSprite()->SetupAttachment(SpritePivot);
	GetSprite()->SetUsingAbsoluteRotation(true);
	GetSprite()->SetRelativeRotation(FRotator(0,0,-90));
}

void APaperVSCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FRotator Rot = GetActorRotation();

	if (-90 < Rot.Yaw && Rot.Yaw < 90)
	{
		SetLookRight(true);
	}
	else if (Rot.Yaw < -90 || 90 < Rot.Yaw)
	{
		SetLookRight(false);
	}
}


UAbilitySystemComponent* APaperVSCharacter::GetAbilitySystemComponent() const
{
	return WeakASC.Get();
}

void APaperVSCharacter::RegisterAbilitySystem(UAbilitySystemComponent* NewASC)
{
	if(NewASC == WeakASC) { return; }

	UnregisterAbilitySystem();

	WeakASC = NewASC;

	if (WeakASC.IsValid())
	{
		bool bFound = false;
		float CurrentMoveSpd = WeakASC->GetGameplayAttributeValue(UAttributeSet_Default::GetMoveSpeedAttribute(), bFound);

		if (bFound)
		{
			SetMoveSpeed(CurrentMoveSpd);
		}

		OnMoveSpeedChangedHandle = WeakASC->GetGameplayAttributeValueChangeDelegate(UAttributeSet_Default::GetMoveSpeedAttribute()).AddUObject(this, &APaperVSCharacter::OnMoveSpeedChanged);
	}

}

void APaperVSCharacter::UnregisterAbilitySystem()
{
	if(!WeakASC.IsValid()) {return;}

	WeakASC->InitAbilityActorInfo(WeakASC->GetOwnerActor(), nullptr);
	WeakASC->GetGameplayAttributeValueChangeDelegate(UAttributeSet_Default::GetMoveSpeedAttribute()).Remove(OnMoveSpeedChangedHandle);

	WeakASC.Reset();
}

void APaperVSCharacter::OnMoveSpeedChanged(const FOnAttributeChangeData& InData)
{
	float ChangedSpeed = InData.NewValue;

	SetMoveSpeed(ChangedSpeed);
}

void APaperVSCharacter::SetMoveSpeed(float NewSpeed)
{
	UCharacterMovementComponent* MovementC = GetCharacterMovement();
	MovementC->MaxWalkSpeed = NewSpeed;
	MovementC->MaxAcceleration = NewSpeed * 16;
}

void APaperVSCharacter::SetLookRight(bool NewState)
{
	if (NewState == bLookRight) return;

	bLookRight = NewState;

	FVector NewScale = bLookRight ? FVector(1, 1, 1) : FVector(-1, 1, 1);

	SpritePivot->SetRelativeScale3D(NewScale);
}