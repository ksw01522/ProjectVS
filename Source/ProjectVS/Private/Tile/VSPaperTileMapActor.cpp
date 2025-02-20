// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile/VSPaperTileMapActor.h"
#include "PaperTileMapComponent.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Components/BrushComponent.h"
#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"
#include "ProjectVS.h"
#include "Components/BoxComponent.h"

AVSPaperTileMapActor::AVSPaperTileMapActor()
{
	TileBox = CreateDefaultSubobject<UBoxComponent>("TileBox");
	SetRootComponent(TileBox);

	GetRenderComponent()->SetupAttachment(TileBox);
	GetRenderComponent()->SetRelativeRotation(FRotator(0,0,-90));
}

void AVSPaperTileMapActor::BeginPlay()
{
	Super::BeginPlay();

	int32 MapWidth, MapHeight, NumLayers;
	GetRenderComponent()->GetMapSize(MapWidth, MapHeight, NumLayers);
	FVector CenterPos = GetRenderComponent()->GetTileCenterPosition(MapWidth * 0.5, MapHeight * 0.5, 0, true);
	FTransform SpawnTransform(FRotator::ZeroRotator, CenterPos, FVector::OneVector);
	ANavMeshBoundsVolume* BoundsVolume = GetWorld()->SpawnActor<ANavMeshBoundsVolume>(ANavMeshBoundsVolume::StaticClass(), SpawnTransform);
	
	FBoxSphereBounds NewBounds = TileBox->GetLocalBounds();
	NewBounds.BoxExtent = FVector(1000,1000,1000) + NewBounds.BoxExtent;
	BoundsVolume->GetRootComponent()->Bounds = TileBox->GetLocalBounds();
	
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	NavSys->Build();
	NavSys->OnNavigationBoundsUpdated(BoundsVolume);

}



FVector AVSPaperTileMapActor::GetTileCornerLocation(EVSTileDirection Dir, bool bWorld)
{
	int32 MapWidth, MapHeight, NumLayers;
	GetRenderComponent()->GetMapSize(MapWidth, MapHeight, NumLayers);

	int X = Dir == EVSTileDirection::TopLeft || Dir == EVSTileDirection::Left || Dir == EVSTileDirection::BottomLeft ? 0:
		Dir == EVSTileDirection::TopCenter || Dir == EVSTileDirection::BottomCenter ? MapWidth * 0.5 : MapWidth;

	int Y = EVSTileDirection::TopLeft <= Dir ? 0 : 
			EVSTileDirection::Left <= Dir ? MapHeight * 0.5 : MapHeight;

	FVector RetLocation = GetRenderComponent()->GetTileCornerPosition(X, Y, 0, true);
	if(!bWorld) { RetLocation -= GetActorLocation(); }

	return RetLocation;
}
