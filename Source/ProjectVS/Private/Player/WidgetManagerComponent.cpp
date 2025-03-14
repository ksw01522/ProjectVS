// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/WidgetManagerComponent.h"
#include "ProjectVSPlayerController.h"
#include "UserWidget.h"

// Sets default values for this component's properties
UWidgetManagerComponent::UWidgetManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWidgetManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	checkf(GetOwner<AProjectVSPlayerController>(), TEXT("Widget Manager Must Add To AProjectVSPlayerController"));
}


// Called every frame
void UWidgetManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//void UWidgetManagerComponent::AddWidgetByServer_Implementation(const FName& InKey, TSubclassOf<UUserWidget> InClass)
//{
//	AddWidget(InKey, InClass);
//}

UUserWidget* UWidgetManagerComponent::AddWidget(const FName& InKey, TSubclassOf<UUserWidget> InClass)
{
	if(InKey.IsNone()) return nullptr;

	UUserWidget* NewWidget = CreateWidget(GetOwner<AProjectVSPlayerController>(), InClass);
	
	return AddWidget(InKey, NewWidget);
}


UUserWidget* UWidgetManagerComponent::AddWidget(const FName& InKey, UUserWidget* InWidget)
{
	if(InKey.IsNone() || InWidget == nullptr) return nullptr;

	WidgetTable.Add(InKey, InWidget);

	return InWidget;
}



void UWidgetManagerComponent::RemoveWidget(const FName& InKey)
{
	UUserWidget* TargetWidget = FindWidget(InKey);
	if(TargetWidget == nullptr) return;

	TargetWidget->RemoveFromParent();
	WidgetTable.Remove(InKey);
}

UUserWidget* UWidgetManagerComponent::FindWidget(const FName& InKey) const
{
	if(!WidgetTable.Contains(InKey)) return nullptr;

	return WidgetTable.FindRef(InKey);
}
