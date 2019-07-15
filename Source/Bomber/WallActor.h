// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include "WallActor.generated.h"

UCLASS()
class BOMBER_API AWallActor final : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWallActor();

	/** The MapComponent manages this actor on the Level Map */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "C++")
	class UMapComponent* MapComponent;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "C++")
	class UStaticMeshComponent* WallMesh;

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() final;

	/** Called when an instance of this class is placed (in editor) or spawned */
	virtual void OnConstruction(const FTransform& Transform) final;

	/** 
	 * Event triggered when the actor has been explicitly destroyed
	 * @warning Should not be destroyed in game
	 */
	UFUNCTION()
	void OnBoxDestroyed(AActor* DestroyedActor);
};
