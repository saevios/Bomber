﻿// Copyright 2020 Yevhenii Selivanov.

#pragma once

#include "LevelActorDataAsset.h"
//---
#include "GameFramework/Character.h"
//---
#include "PlayerCharacter.generated.h"

/**
*
*/
UCLASS(Blueprintable, BlueprintType)
class UPlayerDataAsset final : public ULevelActorDataAsset
{
	GENERATED_BODY()

public:
};

/**
 * Numbers of power-ups that affect the abilities of a player during gameplay.
 */
USTRUCT(BlueprintType)
struct FPowerUp
{
	GENERATED_BODY()

	/** Empty constructor */
	FPowerUp(){};

	/** The number of items, that increases the movement speed of the character */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "C++")
	int32 SkateN = 1;

	/** The number of bombs that can be set at one time */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "C++")
	int32 BombN = 1;

	/** The number of items, that increases the bomb blast radius */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "C++")
	int32 FireN = 1;
};

/**
 * Players and AI, whose goal is to remain the last survivor for the win.
 */
UCLASS()
class BOMBER_API APlayerCharacter final : public ACharacter
{
	GENERATED_BODY()

public:
	/* ---------------------------------------------------
	 *		Public properties
	 * --------------------------------------------------- */
	/** The MapComponent manages this actor on the Level Map */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "C++")
	class UMapComponent* MapComponent;	//[C.AW]

	/** All skeletal meshes of the character */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	TArray<class USkeletalMesh*> SkeletalMeshes;  //[M.DO]

	/** The static mesh nameplate */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "C++")
	class UStaticMeshComponent* NameplateMeshComponent;	 //[C.DO]

	/** All materials that used by nameplate meshes */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++")
	TArray<class UMaterialInterface*> NameplateMaterials;  //[M.DO]

	/* ---------------------------------------------------
	 *		Public functions
	 * --------------------------------------------------- */

	/** Sets default values for this character's properties */
	APlayerCharacter();

	/**  Finds and rotates the self at the current character's location to point at the specified location.
	 * @param Location the character is looking at.
	 * @param bShouldInterpolate if true, smoothly rotate the character toward the direction. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "C++", meta = (AutoCreateRefTerm = "Location"))
	void RotateToLocation(const FVector& Location, bool bShouldInterpolate) const;

protected:
	/* ---------------------------------------------------
	 *		Protected properties
	 * --------------------------------------------------- */

	/** Count of items that affect the abilities of a player during gameplay */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (BlueprintProtected))
	struct FPowerUp Powerups_;	//[AW]
	/** Items have access to increase it */
	friend class AItemActor;
	/** Owned AI controller has access to his pawn to see a blast radius */
	friend class AMyAIController;

	/** The ID identification of each character */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "C++", meta = (BlueprintProtected))
	int32 CharacterID_ = INDEX_NONE;  //[G]

	/* The AnimBlueprint class to use, can set it only in the gameplay */
	class TSubclassOf<UAnimInstance> MyAnimClass;

	/** The character's AI controller */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "C++", meta = (BlueprintProtected))
	class AMyAIController* MyAIController;	//[G]

	/* ---------------------------------------------------
	 *		Protected functions
	 * --------------------------------------------------- */

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** Called when an instance of this class is placed (in editor) or spawned */
	virtual void OnConstruction(const FTransform& Transform) override;

	/* Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Virtual overriding of the UFUNCTION.
	 * Adds the movement input along the given world direction vector.
	 *
	 * @param WorldDirection Direction in world space to apply input
	 * @param ScaleValue Scale to apply to input. This can be used for analog input, ie a value of 0.5 applies half the normal value, while -1.0 would reverse the direction.
	 * @param bForce If true always add the input, ignoring the result of IsMoveInputIgnored().
	 */
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue = 1.0f, bool bForce = false) override;

	/* Move the player character by the forward vector. */
	FORCEINLINE void OnMoveUpDown(float ScaleValue) { AddMovementInput(GetActorForwardVector(), ScaleValue); }

	/* Move the player character by the right vector. */
	FORCEINLINE void OnMoveRightLeft(float ScaleValue) { AddMovementInput(GetActorRightVector(), ScaleValue); }

	/** Spawns bomb on character position */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (BlueprintProtected))
	void SpawnBomb();

	/** Event triggered when the bomb has been explicitly destroyed. */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (BlueprintProtected))
    void OnBombDestroyed(AActor* DestroyedBomb);
};
