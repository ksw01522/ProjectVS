// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectVSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectVSPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "AbilitySystemComponent.h"
#include "Ability/Effect/DamageEffect.h"
#include "Ability/AttributeSet_Default.h"
#include "ProjectVS.h"
#include "Player/VSPlayerState.h"
#include "Ability/VSAbility.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "PaperFlipbookComponent.h"

AProjectVSCharacter::AProjectVSCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	TopDownCameraComponent->SetRelativeRotation(FRotator(0,0,-90.f));

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;


}

void AProjectVSCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


}

void AProjectVSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectVSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInputComponent);

	AProjectVSPlayerController* PlayerController = GetController<AProjectVSPlayerController>();
	check(PlayerController);

	//Moving
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectVSCharacter::Move);

}

void AProjectVSCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);


}

void AProjectVSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (GetLocalRole() != ENetRole::ROLE_Authority) return;

	if (APlayerController* PlayerController = Cast<APlayerController>(NewController))
	{
		AVSPlayerState* VSState = PlayerController->GetPlayerState<AVSPlayerState>();

		UAbilitySystemComponent* PlayerASC = VSState->GetAbilitySystemComponent();

		RegisterAbilitySystem(PlayerASC);

		PlayerASC->InitAbilityActorInfo(VSState, this);

	}
}

void AProjectVSCharacter::UnPossessed()
{
	UnregisterAbilitySystem();

	Super::UnPossessed();
}




FGenericTeamId AProjectVSCharacter::GetGenericTeamId() const
{
	if (AProjectVSPlayerController* PlayerController = GetController<AProjectVSPlayerController>())
	{
		PlayerController->GetGenericTeamId();
	}

	return FGenericTeamId::NoTeam;
}



void AProjectVSCharacter::Move(const FInputActionValue& Value)
{
	if(!IsCanMove()) return;

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(FVector::XAxisVector, MovementVector.X);
		AddMovementInput(FVector::YAxisVector, MovementVector.Y);
	}
}

bool AProjectVSCharacter::IsCanMove() const
{
	UAbilitySystemComponent* PlayerASC = GetAbilitySystemComponent();
	if(PlayerASC == nullptr) return false;

	bool bIsDead = 0 < PlayerASC->GetTagCount(FGameplayTag::RequestGameplayTag("State.Dead"));

	return !bIsDead;
}


