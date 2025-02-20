// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Ability/Gundance/GundanceActor.h"
#include "PaperFlipbookComponent.h"
#include "ProjectVS.h"

#include "Ability/VSAbilityFunctionLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"

#include "GenericTeamAgentInterface.h"

AGundanceActor::AGundanceActor(const FObjectInitializer& ObjectInitializer)
			: Super(ObjectInitializer)
{

	
}

void AGundanceActor::BeginPlay()
{
	Super::BeginPlay();

	UPaperFlipbookComponent* MainFlipbook = GetFlipbookComponent();

	MainFlipbook->OnComponentBeginOverlap.AddDynamic(this, &AGundanceActor::OnBeginOverlapFlipbook);
	MainFlipbook->OnFinishedPlaying.AddDynamic(this, &AGundanceActor::OnFinishedFlipbook);

	MainFlipbook->SetLooping(false);

}

void AGundanceActor::OnFinishedFlipbook()
{
	Destroy();
}

void AGundanceActor::OnBeginOverlapFlipbook(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	LOG_ERROR(TEXT("Test"));

	FGameplayEffectSpecHandle DamageEffectSpecHandle = FindEffectSpec("Damage");
	check(DamageEffectSpecHandle.IsValid())

	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(OtherActor);
	if (ASI == nullptr) return;

	UAbilitySystemComponent* OtherASC = ASI->GetAbilitySystemComponent();

	ETeamAttitude::Type Attitude = UVSAbilityFunctionLibrary::GetTeamAttitude(DamageEffectSpecHandle.Data->GetEffectContext().GetInstigatorAbilitySystemComponent(), OtherASC);
	if (Attitude != ETeamAttitude::Hostile) return;

	OtherASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data);
}
