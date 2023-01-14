﻿// Copyright (c) Yevhenii Selivanov.

#pragma once

#include "Cell.generated.h"

/** Typedef to allow for some nicer looking sets of cells. */
typedef TSet<struct FCell> FCells;

/**
 * Represents one of direction of a cell.
 */
UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ECellDirection : uint8
{
	None = 0,
	Forward = 1 << 0,
	Backward = 1 << 1,
	Right = 1 << 2,
	Left = 1 << 3,
	All = Forward | Backward | Right | Left
};

ENUM_CLASS_FLAGS(ECellDirection);
using ECD = ECellDirection;

/**
 * The structure that contains a location of an one cell (tile) on a grid of the Level Map.
 * X is the column index, Y is the row index on the grid.
 */
USTRUCT(BlueprintType, meta = (HasNativeMake = "/Script/Bomber.CellsUtilsLibrary.MakeCell", HasNativeBreak = "/Script/Bomber.CellsUtilsLibrary.BreakCell"))
struct BOMBER_API FCell
{
	GENERATED_BODY()

	static const FCell InvalidCell;
	static const FCell ForwardCell;
	static const FCell BackwardCell;
	static const FCell RightCell;
	static const FCell LeftCell;
	static const FCells EmptyCells;

	/** The length of the one cell */
	static constexpr float CellSize = 200.f;

	/** Always holds the free cell's FVector-coordinate.
	 * If it is not empty or not found, holds the last succeeded due to copy operator. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++", meta = (ShowOnlyInnerProperties))
	FVector Location = FVector::DownVector;

	/** Default constructor. */
	FCell() = default;

	/** Vector to cell constructor. */
	FCell(const FVector& Vector);

	/** Floats to cell constructor. */
	explicit FCell(float X, float Y, float Z);

	/** Doubles to cell constructor. */
	explicit FCell(double X, double Y, double Z);

	/** Equal operator for vectors to directly copy its value to the cell. */
	FCell& operator=(const FVector& Vector);

	/** Returns Cell's X component. */
	float X() const { return Location.X; }

	/** Returns Cell's Y component. */
	float Y() const { return Location.Y; }

	/** Returns Cell's Z component. */
	float Z() const { return Location.Z; }

	/** Gets a copy of given cell rotated around given transform to the same yaw degree.
	 * @param InCell - The cell to rotate.
	 * @param AxisZ The Z param of the axis to rotate around.
	 * @param OriginTransform The transform of the origin of the rotation. */
	static FCell RotateCellAroundOrigin(const FCell& InCell, float AxisZ, const FTransform& OriginTransform);

	/** Comparing with uninitialized Invalid Cell. */
	FORCEINLINE bool IsInvalidCell() const { return *this == InvalidCell; }

	/** Check is valid this cell. */
	FORCEINLINE bool IsValid() const { return *this != InvalidCell; }

	/** Finds the closest cell to the given cell within array of cells.
	 * @param Cells The array of cells to search in.
	 * @param CellToCheck The start position of the cell to check. */
	static FCell GetCellArrayNearest(const TSet<FCell>& Cells, const FCell& CellToCheck);

	/** Returns the width (columns X) and the length (rows Y) in specified cells, where each 1 unit means 1 cell.
	 * E.g: if given cells are corner cells on 7x9 level, it will return 7 columns (X) and 9 rows (Y) columns respectively. */
	static float GetCellArrayWidth(const FCells& InCells);
	static float GetCellArrayLength(const FCells& InCells);

	/** Constructs and returns new grid from given transform.
	 * @param OriginTransform its location and rotation is the center of new grid, its scale-X is number of columns, scale-Y is number of rows. */
	static TSet<FCell> MakeCellGridByTransform(const FTransform& OriginTransform);

	/** Allows rotate or unrotated given grid around its origin. */
	static FCells RotateCellArray(float AxisZ, const FCells& InCells);

	/** Makes origin transform for given grid. */
	static FTransform GetCellArrayTransform(const FCells& InCells);

	/** Makes rotator for given grid its origin. */
	static FRotator GetCellArrayRotator(const FCells& InCells);

	/** Returns how many cells are between two cells, where each 1 unit means one cell. */
	template <typename T>
	static FORCEINLINE T Distance(const FCell& C1, const FCell& C2) { return FMath::Abs<T>((C1.Location - C2.Location).Size()) / CellSize; }

	/** Find the max distance between cells within specified set, where each 1 unit means one cell. */
	template <typename T>
	static T GetCellArrayMaxDistance(const FCells& Cells);

	/**
	 * Compares cells for equality.
	 *
	 * @param Other The other cell being compared.
	 * @return true if the points are equal, false otherwise
	 */
	FORCEINLINE bool operator==(const FCell& Other) const { return this->Location == Other.Location; }
	FORCEINLINE bool operator!=(const FCell& Other) const { return !(*this == Other); }

	/** Addition of cells. */
	FCell& operator+=(const FCell& Other);
	friend FORCEINLINE FCell operator+(const FCell& Lhs, const FCell& Rhs) { return FCell(Lhs.Location + Rhs.Location); }

	/** Subtracts a cell from another cell. */
	FCell& operator-=(const FCell& Other);
	friend FORCEINLINE FCell operator-(const FCell& Lhs, const FCell& Rhs) { return FCell(Lhs.Location - Rhs.Location); }

	/** Vector operator to return cell location. */
	FORCEINLINE operator FVector() const { return this->Location; }

	/** Find the average of an set of cells. */
	static FCell GetCellArrayAverage(const FCells& Cells);

	/** Returns the cell direction by its enum. */
	static const FCell& GetCellDirection(ECellDirection CellDirection);
	static ECellDirection GetCellDirection(const FCell& CellDirection);

	/** Puts specified cell in the cells set.*/
	static FORCEINLINE FCells CellToCells(const FCell& InCell) { return FCells{InCell}; }
	FCells ToCells() const { return CellToCells(*this); }

	/** Converts set of cells to array of vectors and vice versa. */
	static TArray<FVector> CellsToVectors(const FCells& Cells);
	static FCells VectorsToCells(const TArray<FVector>& Vectors);

	/** Extracts first cell from specified cells set.*/
	static FORCEINLINE FCell GetFirstCellInSet(const FCells& InCells) { return !InCells.IsEmpty() ? InCells.Array()[0] : InvalidCell; }

	/** Gets a copy of given cell snapped its location to a grid while it does not respect rotated grids. */
	static FORCEINLINE FCell SnapCell(const FCell& InCell) { return InCell.Location.GridSnap(CellSize); }

	/** Gets a copy of given cell snapped to the grid by its origin transform, so it makes possible to snap to the rotated grid. */
	static FCell SnapRotatedCell(const FCell& InCell, const FTransform& GridOriginTransform);

	/**
	* Creates a hash value from a FCell.
	*
	* @param Vector the cell to create a hash value for
	* @return The hash value from the components
	*/
	friend FORCEINLINE uint32 GetTypeHash(const FCell& Vector) { return GetTypeHash(Vector.Location); }
};

// Find the max distance between cells within specified set, where each 1 unit means one cell
template <typename T>
T FCell::GetCellArrayMaxDistance(const FCells& Cells)
{
	T MaxDistance{};
	for (const FCell& C1 : Cells)
	{
		for (const FCell& C2 : Cells)
		{
			const T LengthIt = FCell::Distance<T>(C1, C2);
			if (LengthIt > MaxDistance)
			{
				MaxDistance = LengthIt;
			}
		}
	}
	return MaxDistance;
}
