// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapActor.h"
#include "VSPaperTileMapActor.generated.h"

/**
 * 
 */
UENUM()
enum class EVSTileDirection : uint8
{
	None = 0 UMETA(Hidden),
	BottomLeft = 1,
	BottomCenter = 2,
	BottomRight = 3,
	Left = 4,
	Right = 5,
	TopLeft = 6,
	TopCenter = 7,
	TopRight = 8
};

ENUM_RANGE_BY_FIRST_AND_LAST(EVSTileDirection, EVSTileDirection::BottomLeft, EVSTileDirection::TopRight);

UCLASS()
class PROJECTVS_API AVSPaperTileMapActor : public APaperTileMapActor
{
	GENERATED_BODY()
	
public:
	AVSPaperTileMapActor();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Pivot")
	TObjectPtr<class UBoxComponent> TileBox;

protected:
	virtual void BeginPlay() override;

public:
	UBoxComponent* GetTileBox() const {return TileBox;}

	FVector GetTileCornerLocation(EVSTileDirection Dir, bool bWorld = true);
};
