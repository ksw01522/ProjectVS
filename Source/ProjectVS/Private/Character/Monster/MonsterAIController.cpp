// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Monster/MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AMonsterAIController::AMonsterAIController()
{
	PrimaryActorTick.bCanEverTick = true;


	SetGenericTeamId(2);
}

void AMonsterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

}

void AMonsterAIController::OnUnPossess()
{

	Super::OnUnPossess();
}

void AMonsterAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AMonsterAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation());

}
