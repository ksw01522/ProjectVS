// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "VSAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVS_API UVSAnimInstance : public UPaperZDAnimInstance
{
	GENERATED_BODY()
public:
	UVSAnimInstance();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VSAnim", meta = (AllowPrivateAccess = "true"))
	float Velocity = 0;

protected:
	virtual void OnTick_Implementation(float DeltaTime) override;

};
