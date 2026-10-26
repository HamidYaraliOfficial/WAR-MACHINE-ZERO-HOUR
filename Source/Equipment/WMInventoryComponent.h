#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WMInventoryComponent.generated.h"

UENUM(BlueprintType)
enum class EWMLoadoutSlot : uint8
{
	Primary, Secondary, Utility, Medical, ReconTool, ArmorConfiguration
};

USTRUCT(BlueprintType)
struct FWMLoadoutItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EWMLoadoutSlot Slot = EWMLoadoutSlot::Primary;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float WeightKg = 1.0f;
};

USTRUCT(BlueprintType)
struct FWMLoadoutPreset
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName PresetName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FWMLoadoutItem> Items;
};

/** Player/operator inventory: slot-based loadout with a computed total weight that feeds movement handling. */
UCLASS(ClassGroup = (WarMachine), meta = (BlueprintSpawnableComponent))
class UWMInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UWMInventoryComponent();

	UPROPERTY(BlueprintReadOnly, Category = "Inventory") TArray<FWMLoadoutItem> EquippedItems;
	UPROPERTY(BlueprintReadOnly, Category = "Inventory") TArray<FWMLoadoutPreset> SavedPresets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory") float MaxCarryWeightKg = 40.0f;

	UFUNCTION(BlueprintCallable, Category = "Inventory") bool EquipItem(EWMLoadoutSlot Slot, FName ItemId, float WeightKg);
	UFUNCTION(BlueprintCallable, Category = "Inventory") void UnequipSlot(EWMLoadoutSlot Slot);
	UFUNCTION(BlueprintCallable, Category = "Inventory") void SavePreset(FName PresetName);
	UFUNCTION(BlueprintCallable, Category = "Inventory") bool LoadPreset(FName PresetName);
	UFUNCTION(BlueprintPure, Category = "Inventory") float GetTotalWeightKg() const;
	/** 0..1 handling multiplier derived from carry weight vs capacity (heavier = slower ADS/sprint/reload). */
	UFUNCTION(BlueprintPure, Category = "Inventory") float GetHandlingPenalty() const;
};
