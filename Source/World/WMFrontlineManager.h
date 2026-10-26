#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WMTypes.h"
#include "WMFrontlineManager.generated.h"

USTRUCT(BlueprintType)
struct FWMFrontlineSector
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) FName SectorId;
	UPROPERTY(BlueprintReadOnly) FVector Location = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly) EWMFaction ControllingFaction = EWMFaction::None;
	UPROPERTY(BlueprintReadOnly) float ContestedPressure = 0.0f; // 0 = stable, 1 = actively contested
};

/** Tracks which sectors are held by whom and where the line is actively moving; missions are drawn from here. */
UCLASS()
class UWMFrontlineManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Frontline")
	TArray<FWMFrontlineSector> Sectors;

	UFUNCTION(BlueprintCallable, Category = "Frontline")
	void RegisterSector(FName SectorId, FVector Location, EWMFaction InitialOwner);

	UFUNCTION(BlueprintCallable, Category = "Frontline")
	void ApplyPressure(FName SectorId, float PressureDelta, EWMFaction Attacker);

	UFUNCTION(BlueprintCallable, Category = "Frontline")
	void ResolveContestedSectors();

	UFUNCTION(BlueprintPure, Category = "Frontline")
	FWMFrontlineSector GetMostContestedSector() const;
};
