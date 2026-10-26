#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WMTypes.h"
#include "WMFactionData.generated.h"

/** Data-driven description of a fictional faction: doctrine, colors, radio callsigns. No real-world army is modeled. */
UCLASS(BlueprintType)
class UWMFactionDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Faction")
	EWMFaction FactionId = EWMFaction::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Faction")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Faction")
	FLinearColor PrimaryColor = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Faction")
	FLinearColor SecondaryColor = FLinearColor::Gray;

	/** Aggressiveness bias used by the AI Commander (0 = defensive, 1 = highly aggressive). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Doctrine", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float AggressionBias = 0.5f;

	/** Preference for consolidating defensive lines vs. maneuver warfare. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Doctrine", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float DefensiveBias = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Radio")
	TArray<FString> RadioCallsigns;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Doctrine")
	TArray<TSoftClassPtr<UObject>> PreferredVehicleClasses;
};
