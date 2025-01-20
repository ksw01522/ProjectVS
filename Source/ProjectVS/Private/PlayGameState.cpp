// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayGameState.h"
#include "Net/UnrealNetwork.h"
#include "ProjectVS.h"

APlayGameState::APlayGameState()
{
	StageState = EStageState::Idle;
}

void APlayGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(APlayGameState, StageState, COND_None, REPNOTIFY_OnChanged);

	DOREPLIFETIME_CONDITION_NOTIFY(APlayGameState, MonsterKillCount, COND_None, REPNOTIFY_OnChanged);

	DOREPLIFETIME_CONDITION_NOTIFY(APlayGameState, StageTime, COND_None, REPNOTIFY_OnChanged);
}

void APlayGameState::OnRep_StageState(EStageState OldState)
{
	
}

void APlayGameState::SetStageState(EStageState NewState)
{
	if(StageState == NewState) return;

	EStageState OldState = StageState;
	StageState = NewState;

	OnRep_StageState(OldState);
}

void APlayGameState::OnRep_MonsterKillCount(int OldMonsterKillCount)
{
	OnMonsterKillCountChanged.Broadcast(OldMonsterKillCount, MonsterKillCount);
}

void APlayGameState::OnRep_StageTime(float OldStageTime)
{
	OnStageTimeChangedDelegate.Broadcast(OldStageTime, StageTime);
}

void APlayGameState::UpdateStageTime(float InTime)
{
	if(InTime == 0) return;

 	float OldStageTime = StageTime;
	StageTime += InTime;

	OnRep_StageTime(OldStageTime);
}

void APlayGameState::SetStageTime(float NewTime)
{
	if (NewTime == StageTime) return;

	float OldStageTime = StageTime;
	StageTime = NewTime;

	OnRep_StageTime(OldStageTime);
}



void APlayGameState::SetMonsteKillCount(int NewCount)
{
	if(MonsterKillCount == NewCount) return;

	int OldMonsterKillCount = MonsterKillCount;
	MonsterKillCount = NewCount;

	OnRep_MonsterKillCount(OldMonsterKillCount);
}
