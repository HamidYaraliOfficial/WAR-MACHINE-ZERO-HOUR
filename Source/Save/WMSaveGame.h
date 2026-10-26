#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WMTypes.h"
#include "WMWarStateSubsystem.h"
#include "WMSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FWMSquadMemberSaveData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite) FGuid MemberId;
	UPROPERTY(BlueprintReadWrite) FString DisplayName;
	UPROPERTY(BlueprintReadWrite) EWMUnitRole Role = EWMUnitRole::Infantry;
	UPROPERTY(BlueprintReadWrite) EWMUnitState UnitState = EWMUnitState::Healthy;
	UPROPERTY(BlueprintReadWrite) int32 Experience = 0;
	UPROPERTY(BlueprintReadWrite) TArray<FName> UnlockedTraits;
};

USTRUCT(BlueprintType)
struct FWMSectorSaveData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite) FName SectorId;
	UPROPERTY(BlueprintReadWrite) EWMFaction ControllingFaction = EWMFaction::None;
	UPROPERTY(BlueprintReadWrite) float ContestedPressure = 0.0f;
};

/**
 * Full campaign save payload. Versioned so future builds can migrate older saves rather than
 * rejecting them outright, and validated on load before being applied to live subsystems.
 */
UCLASS()
class UWMSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "Save") int32 SaveVersion = 1;
	UPROPERTY(BlueprintReadWrite, Category = "Save") FDateTime SavedAtUtc;
	UPROPERTY(BlueprintReadWrite, Category = "Save") FString SlotName;

	UPROPERTY(BlueprintReadWrite, Category = "Save") FWMWarIndices WarIndices;
	UPROPERTY(BlueprintReadWrite, Category = "Save") TArray<FWMSectorSaveData> Sectors;
	UPROPERTY(BlueprintReadWrite, Category = "Save") TArray<FWMSquadMemberSaveData> SquadMembers;
	UPROPERTY(BlueprintReadWrite, Category = "Save") TArray<FName> CompletedObjectiveTags;
	UPROPERTY(BlueprintReadWrite, Category = "Save") TArray<FName> DiscoveredIntelTags;
	UPROPERTY(BlueprintReadWrite, Category = "Save") int32 ResourcePoints = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Save") EWMDifficulty Difficulty = EWMDifficulty::Regular;

	/** Simple structural check used before applying a loaded save to the live world. */
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool ValidateIntegrity() const
	{
		if (SaveVersion <= 0) return false;
		if (SlotName.IsEmpty()) return false;
		return true;
	}
};
