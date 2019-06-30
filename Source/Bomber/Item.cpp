// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"

#include "Bomber.h"
#include "MapComponent.h"
#include "MyCharacter.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	// Initialize MapComponent
	MapComponent = CreateDefaultSubobject<UMapComponent>(TEXT("Map Component"));
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &AItem::OnItemBeginOverlap);
}

void AItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITOR
	if (HasActorBegunPlay() == false)  // for editor only
	{
		// Update dragged actor
		if (IS_VALID(MapComponent) == true)  // Map component is valid
		{
			MapComponent->UpdateSelfOnMap();
		}
		else  // Transient actor
		{
			return;
		}
	}
#endif  //WITH_EDITOR
}

void AItem::OnItemBeginOverlap(AActor* OverlappedItem, AActor* OtherActor)
{
	if (OverlappedItem == this						   // self triggering
		|| Cast<AMyCharacter>(OtherActor) == nullptr)  // other actor is not myCharacter
	{
		return;
	}
}
