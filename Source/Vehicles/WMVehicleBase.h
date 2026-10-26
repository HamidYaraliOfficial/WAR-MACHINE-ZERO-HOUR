#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "WMTypes.h"
#include "WMVehicleBase.generated.h"

class UWMVehicleDamageComponent;

UENUM(BlueprintType)
enum class EWMVehicleClass : uint8
{
	Transport, ArmoredVehicle, HeavyArmor, ReconVehicle, SupplyTruck, EngineeringVehicle, AirSupportPlatform
};

/**
 * Base for all fictional vehicles. Physics (mass/accel/traction/suspension) is delegated to the engine's
 * Chaos Vehicles plugin via a movement component set up per-subclass in Blueprint; this base class owns
 * the gameplay layer: fuel/energy, crew seats and the link to WMVehicleDamageComponent.
 */
UCLASS(Abstract, Blueprintable)
class AWMVehicleBase : public APawn
{
	GENERATED_BODY()
public:
	AWMVehicleBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	EWMVehicleClass VehicleClass = EWMVehicleClass::Transport;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Vehicle")
	TObjectPtr<UWMVehicleDamageComponent> DamageComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	float MaxFuelOrEnergy = 100.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Vehicle")
	float CurrentFuelOrEnergy = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Vehicle")
	int32 MaxCrewSeats = 4;

	UPROPERTY(BlueprintReadOnly, Category = "Vehicle")
	TArray<TObjectPtr<APawn>> OccupyingCrew;

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	bool AssignCrew(APawn* Crewman);

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void ConsumeFuel(float Amount) { CurrentFuelOrEnergy = FMath::Max(0.0f, CurrentFuelOrEnergy - Amount); }

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void Refuel(float Amount) { CurrentFuelOrEnergy = FMath::Min(MaxFuelOrEnergy, CurrentFuelOrEnergy + Amount); }

	UFUNCTION(BlueprintPure, Category = "Vehicle")
	bool IsOperational() const;
};
