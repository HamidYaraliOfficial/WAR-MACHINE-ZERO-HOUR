#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WMDroneComponent.generated.h"

UENUM(BlueprintType)
enum class EWMDroneFlightMode : uint8
{
	Grounded, Hovering, Cruising, Returning
};

/** A recon drone with real gameplay limitations: battery, range, signal quality and damage all constrain use. */
UCLASS(ClassGroup = (WarMachine), meta = (BlueprintSpawnableComponent))
class UWMDroneComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UWMDroneComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone") float MaxBattery = 100.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Drone") float CurrentBattery = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone") float MaxRangeMeters = 800.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone") FVector HomeLocation = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly, Category = "Drone") float SignalQuality = 1.0f; // degrades with distance/obstruction
	UPROPERTY(BlueprintReadOnly, Category = "Drone") float DamageState = 0.0f;  // 0 = pristine, 1 = destroyed
	UPROPERTY(BlueprintReadOnly, Category = "Drone") EWMDroneFlightMode FlightMode = EWMDroneFlightMode::Grounded;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drone") bool bHasThermalSensor = true;

	UFUNCTION(BlueprintCallable, Category = "Drone") void Launch();
	UFUNCTION(BlueprintCallable, Category = "Drone") void TickFlight(float DeltaSeconds, FVector CurrentLocation, float SignalObstruction01);
	UFUNCTION(BlueprintCallable, Category = "Drone") void ApplyDamage(float Amount);
	UFUNCTION(BlueprintPure, Category = "Drone") bool CanOperate() const { return CurrentBattery > 0.0f && DamageState < 1.0f; }
};
