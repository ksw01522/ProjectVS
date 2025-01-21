// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WidgetManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTVS_API UWidgetManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWidgetManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(Transient)
	TMap<FName, TObjectPtr<class UUserWidget>> WidgetTable;

public:
	//UFUNCTION(Client, Reliable)
	//void AddWidgetByServer(const FName& InKey, TSubclassOf<UUserWidget> InClass);
	//void AddWidgetByServer_Implementation(const FName& InKey, TSubclassOf<UUserWidget> InClass);


	UUserWidget* AddWidget(const FName& InKey, TSubclassOf<UUserWidget> InClass);

	UUserWidget* AddWidget(const FName& InKey, UUserWidget* InWidget);

	void RemoveWidget(const FName& InKey);
	
	UUserWidget* FindWidget(const FName& InKey) const;

};
