#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WMTacticalMapSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FWMMapContact
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) FGuid ContactId;
	UPROPERTY(BlueprintReadOnly) FVector LastKnownLocation = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly) float Confidence = 1.0f; // 1 = confirmed now, decays over time
	UPROPERTY(BlueprintReadOnly) float TimestampSeconds = 0.0f;
	UPROPERTY(BlueprintReadOnly) bool bIsHostile = true;
	UPROPERTY(BlueprintReadOnly) FName SourceTag; // "Drone", "Scout", "SquadReport", "Radio", "Visual"
};

/**
 * Owns the player's shared operational picture: friendly units, known/unknown contacts, objectives,
 * supply points and threat zones. Confidence decays over time to represent Fog of War / stale intel,
 * so the map never reveals ground truth the player hasn't actually earned via Recon/Reports.
 */
UCLASS()
class UWMTacticalMapSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "TacticalMap")
	void ReportContact(FVector Location, bool bHostile, FName SourceTag, float Confidence = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "TacticalMap")
	void DecayContacts(float DeltaSeconds, float DecayRatePerSecond = 0.02f);

	UFUNCTION(BlueprintPure, Category = "TacticalMap")
	const TArray<FWMMapContact>& GetKnownContacts() const { return Contacts; }

	UFUNCTION(BlueprintCallable, Category = "TacticalMap")
	void ClearStaleContacts(float MinConfidence = 0.05f);

private:
	UPROPERTY()
	TArray<FWMMapContact> Contacts;
};
