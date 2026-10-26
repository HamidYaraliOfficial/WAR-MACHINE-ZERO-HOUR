#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WMSaveGame.h"
#include "WMSaveSubsystem.generated.h"

/**
 * Save/Load orchestration: multiple slots, auto-save, manual save, integrity validation and a
 * crash-recovery slot that is written immediately before risky transitions (mission start/extraction).
 */
UCLASS()
class UWMSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	static const FString CrashRecoverySlotName;

	UFUNCTION(BlueprintCallable, Category = "Save")
	bool SaveToSlot(UWMSaveGame* SaveData, const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Save")
	UWMSaveGame* LoadFromSlot(const FString& SlotName, bool& bOutSuccess);

	UFUNCTION(BlueprintCallable, Category = "Save")
	void WriteCrashRecoveryCheckpoint(UWMSaveGame* SaveData);

	UFUNCTION(BlueprintCallable, Category = "Save")
	TArray<FString> ListAvailableSlots() const;

	UFUNCTION(BlueprintCallable, Category = "Save")
	bool DeleteSlot(const FString& SlotName);
};
