// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityTesterUtilityWidget.h"
#include "AbilityTesterEditor.h"
#include "EngineUtils.h"

#include "Components/Viewport.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Widgets/SViewport.h"


#include "Input/Reply.h"

#include "AbilityTesterActor.h"
#include "Engine/StaticMeshActor.h"

#include "Components/DetailsView.h"

#include "Components/Button.h"

#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/CompilerResultsLog.h"
#include "FileHelpers.h"

#include "GameplayAbility.h"
#include "AbilityEditorSubsystem.h"
#include "InputCoreTypes.h"
#include "Components/SinglePropertyView.h"

#define LOCTEXT_NAMESPACE "AbilityTester"

void UAbilityTesterUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitViewport();
	UtilityDetails->CategoriesToShow.Add(TEXT("TestAbility"));
	UtilityDetails->CategoriesToShow.Add(TEXT("View"));
	UtilityDetails->SetObject(this);
	
	TestLevelDetail->SetPropertyName(GET_MEMBER_NAME_CHECKED(UAbilityTesterUtilityWidget, TestLevel));
	TestLevelDetail->SetObject(this);

	ApplyTestAbility();

	ApplyBTN->OnClicked.AddDynamic(this, &UAbilityTesterUtilityWidget::OnClickedApplyBTN);
	SaveBTN->OnClicked.AddDynamic(this, &UAbilityTesterUtilityWidget::OnClickedSaveBTN);
}

void UAbilityTesterUtilityWidget::NativeDestruct()
{

	Super::NativeDestruct();
}

void UAbilityTesterUtilityWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


}

void UAbilityTesterUtilityWidget::InitViewport()
{
	TesterViewport = NewObject<UViewport>(this);
	ViewportBorder->AddChild(TesterViewport);
	
	TesterViewport->SetViewLocation(ViewLocation);
	TesterViewport->SetViewRotation(ViewRotation);

	SpawnTesterActor();
	
	//TSharedPtr<SViewport> ViewportSlate = StaticCastSharedPtr<SViewport>(TesterViewport->GetCachedWidget());
}

void UAbilityTesterUtilityWidget::SpawnTesterActor()
{
	if (IsValid(TesterActor))
	{
		TesterActor->Destroy();
	}

	TesterActor = Cast<AAbilityTesterActor>(TesterViewport->Spawn(TesterActorClass));
}

void UAbilityTesterUtilityWidget::OnClickedApplyBTN()
{
	ApplyTestAbility();
}

void UAbilityTesterUtilityWidget::OnClickedSaveBTN()
{
	UPackage* PackageToSave = TestAbility->GetPackage();
	if (PackageToSave == nullptr)
	{
		return;
	}

	FString BPPath = PackageToSave->GetPathName();
	UBlueprint* BPToCompile = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), nullptr, *BPPath));
	
	FCompilerResultsLog Results;
	FKismetEditorUtilities::CompileBlueprint(BPToCompile, EBlueprintCompileOptions::SkipSave, &Results);
	
	for (TSharedRef<FTokenizedMessage> Message : Results.Messages)
	{
		if	(Message.Get().GetSeverity() != EMessageSeverity::Error)
		{ continue; }
		
		StateTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		StateTextBlock->SetText(Message.Get().ToText());
		return;
	}
	
	if (UEditorLoadingAndSavingUtils::SavePackages({PackageToSave}, false))
	{
		StateTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		StateTextBlock->SetText(LOCTEXT("Succeed Save AbilityArray", "Succeed Save Ability"));

	}
	else
	{
		StateTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
		StateTextBlock->SetText(LOCTEXT("Fail Save AbilityArray", "Fail Save Ability"));
	}
}


void UAbilityTesterUtilityWidget::OnChangedTestAbility()
{
	AbilityDetails->SetObject(TestAbility);

}

void UAbilityTesterUtilityWidget::ApplyTestAbility()
{
	if (TesterActor)
	{
		TesterActor->SetTestAbility(TestAbility, TestLevel);
	}
}


void UAbilityTesterUtilityWidget::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UAbilityTesterUtilityWidget, TesterActorClass))
	{
		SpawnTesterActor();
	}
	else if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UAbilityTesterUtilityWidget, TestAbility))
	{
		OnChangedTestAbility();
		ApplyTestAbility();
	}
	else if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UAbilityTesterUtilityWidget, TestLevel))
	{
		ApplyTestAbility();
	} 
	else if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UAbilityTesterUtilityWidget, ViewLocation))
	{
		if (TesterViewport)
		{
			TesterViewport->SetViewLocation(ViewLocation);
		}
	}
	else if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UAbilityTesterUtilityWidget, ViewRotation))
	{
		if (TesterViewport)
		{
			TesterViewport->SetViewRotation(ViewRotation);
		}
	}
}

#undef LOCTEXT_NAMESPACE