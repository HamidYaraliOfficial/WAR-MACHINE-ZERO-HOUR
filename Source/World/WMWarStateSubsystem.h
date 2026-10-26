#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WMTypes.h"
#include "WMWarStateSubsystem.generated.h"

/** Aggregate indices describing the overall war; future missions are selected/weighted from this state. */
USTRUCT(BlueprintType)
struct FWMWarIndices
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) float TerritoryControl = 0.5f;   // 0 = enemy dominant, 1 = player faction dominant
	UPROPERTY(BlueprintReadOnly) float SupplyStrength = 0.7f;
	UPROPERTY(BlueprintReadOnly) float IntelligenceQuality = 0.4f;
	UPROPERTY(BlueprintReadOnly) float Morale = 0.6f;
	UPROPERTY(BlueprintReadOnly) float InfrastructureIntegrity = 0.8f;
	UPROPERTY(BlueprintReadOnly) float MilitaryStrength = 0.65f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWarIndicesChanged, const FWMWarIndices&, NewIndices);

/** Tracks the persistent, campaign-wide state of the war and how player decisions permanently shift it. */
UCLASS()
class UWMWarStateSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "WarState")
	FWMWarIndices Indices;

	UPROPERTY(BlueprintAssignable, Category = "WarState")
	FOnWarIndicesChanged OnWarIndicesChanged;

	UFUNCTION(BlueprintCallable, Category = "WarState")
	void InitializeWarState();

	UFUNCTION(BlueprintCallable, Category = "WarState")
	void ApplyMissionOutcome(bool bSuccess, float TerritoryDelta, float SupplyDelta, float IntelDelta);

	/** Called when the player skips/fails an optional objective: quietly nudges future mission difficulty/state. */
	UFUNCTION(BlueprintCallable, Category = "WarState")
	void RecordObjectiveDecision(EWMObjectiveType Type, bool bWasSkipped);

	UFUNCTION(BlueprintPure, Category = "WarState")
	float GetOverallWarScore() const;

private:
	void ClampAndBroadcast();
};
