// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnRule/SpawnRule.h"
#include "MonsterWorldManager.h"
#include "Misc/DataValidation.h"

USpawnRule::USpawnRule()
{

}

void USpawnRule::CreateSpawnLocations(TArray<FVector>& OutLocations, const FSpawnData& SpawnData, UWorld& InWorld)
{
	UMonsterWorldManager* MonsterWorldManager = InWorld.GetSubsystem<UMonsterWorldManager>();
	if (MonsterWorldManager == nullptr) return;


}

void USpawnRule::SpawnMonster(UWorld& InWorld)
{
	UMonsterWorldManager* MonsterWorldManager = InWorld.GetSubsystem<UMonsterWorldManager>();
	if(MonsterWorldManager == nullptr) return;

	for (const auto& SpawnData : MonsterSpawnDatas)
	{
		TArray<FVector> SpawnLocations;

		CreateSpawnLocations(SpawnLocations, SpawnData, InWorld);

		MonsterWorldManager->SpawnMonseters(SpawnData.Code, SpawnData.Level, SpawnLocations);
	}
}

USpawnRule_RandomInDonut::USpawnRule_RandomInDonut()
{
	InnerCircleRadius = 1000;
	OuterCircleRadius = 1200;
}

#if WITH_EDITOR
EDataValidationResult USpawnRule_RandomInDonut::IsDataValid(FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	if (OuterCircleRadius < InnerCircleRadius)
	{
		FFormatNamedArguments Args;
		
		Args.Add("InnerRadius", InnerCircleRadius);
		Args.Add("OuterRadius", OuterCircleRadius);

		FText RadiusErrorText = FText::Format(NSLOCTEXT("ProjectVSEditor", "Donut Radius Error", "InnerCircleRadius[{InnerRadius}] Greater OuterCircleRadius[{OuterRadius}]"), Args);

		Context.AddError(RadiusErrorText);
	}

	return 0 < Context.GetNumErrors() ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}

#endif

void USpawnRule_RandomInDonut::CreateSpawnLocations(TArray<FVector>& OutLocations, const FSpawnData& SpawnData, UWorld& InWorld)
{
	float BaseDegree = FMath::RandRange(0.0, 360.0);

	float IntervalDegree = FMath::Min((float)360 / (float)SpawnData.Count, 10.0);

	FVector SpawnCenterLocation = InWorld.GetFirstPlayerController()->GetPawn()->GetActorLocation();

	for (int i = 0; i < SpawnData.Count; i++)
	{
		float TempDegree = BaseDegree + IntervalDegree * i + FMath::RandRange(-3, 3);
		float RandDist = FMath::RandRange(InnerCircleRadius, OuterCircleRadius);

		FVector NewLocation = FVector::XAxisVector.RotateAngleAxis(TempDegree, FVector::ZAxisVector);
		NewLocation *= RandDist;

		NewLocation += SpawnCenterLocation;
		OutLocations.Add(NewLocation);
	}
}
