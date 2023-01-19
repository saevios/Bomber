﻿// Copyright (c) Yevhenii Selivanov

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
//---
#include "AssetTypeActions_Base.h"
#include "IAssetTools.h"
//---
#include "EditorUtilsLibrary.generated.h"

/**
 * The editor functions library
 */
UCLASS()
class MYEDITORUTILS_API UEditorUtilsLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

#pragma region PIE
	/** Checks, is the current world placed in the editor. */
	UFUNCTION(BlueprintPure, Category = "C++", meta = (DevelopmentOnly))
	static bool IsEditor();

	/** Checks is the current world placed in the editor and the game not started yet. */
	UFUNCTION(BlueprintPure, Category = "C++", meta = (DevelopmentOnly))
	static bool IsEditorNotPieWorld();

	/** Returns true if game is started in the Editor. */
	UFUNCTION(BlueprintPure, Category = "C++", meta = (DevelopmentOnly))
	static bool IsPIE();

	/** Returns true if is started multiplayer game (server + client(s)) right in the Editor. */
	UFUNCTION(BlueprintPure, Category = "C++", meta = (DevelopmentOnly))
	static bool IsEditorMultiplayer();

	/** Returns the index of current player during editor multiplayer.
	 * 0 is server.
	 * 1 (or higher) is client.
	 * -1 in the standalone game. */
	UFUNCTION(BlueprintPure, Category = "C++", meta = (DevelopmentOnly))
	static int32 GetEditorPlayerIndex();
#pragma endregion PIE

	/** Returns current editor viewport. */
	static class FViewport* GetEditorViewport();

	/** Exports specified data table to already its .json. */
	UFUNCTION(BlueprintCallable, Category = "C++", meta = (DevelopmentOnly))
	static void ReExportTableAsJSON(const class UDataTable* DataTable);

#pragma region EditorExtensions
	/** Adds the asset to the context menu
	 * @param InOutRegisteredAssets Input: the list of registered assets. Output: the list of registered assets + the new one. */
	template <typename T>
	static void RegisterAsset(TArray<TSharedPtr<FAssetTypeActions_Base>>& InOutRegisteredAssets);

	/** Removes all custom assets from context menu. */
	static void UnregisterAssets(TArray<TSharedPtr<FAssetTypeActions_Base>>& RegisteredAssets);
#pragma endregion EditorExtensions
};

// Adds the asset to the context menu
template <typename T>
void UEditorUtilsLibrary::RegisterAsset(TArray<TSharedPtr<FAssetTypeActions_Base>>& InOutRegisteredAssets)
{
	TSharedPtr<T> SettingsDataTableAction = MakeShared<T>();
	IAssetTools::Get().RegisterAssetTypeActions(SettingsDataTableAction.ToSharedRef());
	InOutRegisteredAssets.Emplace(MoveTemp(SettingsDataTableAction));
}
