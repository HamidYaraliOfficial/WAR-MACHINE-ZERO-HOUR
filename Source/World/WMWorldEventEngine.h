#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WMWorldEventEngine.generated.h"

UENUM(BlueprintType)
enum class EWMWorldEventType : uint8
{
	ReinforcementArrival, ConvoyDelay, CommunicationsFailure, WeatherShift,
	CivilianEvacuation, InfrastructureDamage, ReconDiscovery, EnemyMovement
};

USTRUCT(BlueprintType)
struct FWMWorldEvent
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly) EWMWorldEventType EventType = EWMWorldEventType::EnemyMovement;
	UPROPERTY(BlueprintReadOnly) FVector Location = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly) float TimestampSeconds = 0.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorldEventTriggered, const FWMWorldEvent&, Event);

/**
 * Generates context-aware world events from current War State / Frontline / Weather / Logistics conditions.
 * Events are weighted by state, not drawn from a flat uniform random table, so low supply raises the odds
 * of a ConvoyDelay, poor comms infrastructure raises CommunicationsFailure odds, and so on.
 */
UCLASS()
class UWMWorldEventEngine : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = "WorldEvents")
	FOnWorldEventTriggered OnWorldEventTriggered;

	UFUNCTION(BlueprintCallable, Category = "WorldEvents")
	void EvaluateTick(float SupplyStrength, float InfrastructureIntegrity, float IntelligenceQuality, float DeltaSeconds);

private:
	float AccumulatedSeconds = 0.0f;
	void FireEvent(EWMWorldEventType Type, FVector Location);
};
