#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WMTypes.h"
#include "WMVehicleDamageComponent.generated.h"

USTRUCT(BlueprintType)
struct FWMVehicleComponentState
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly) EWMVehicleComponent Component = EWMVehicleComponent::Engine;
	UPROPERTY(BlueprintReadOnly) float Integrity = 1.0f; // 0..1
	UPROPERTY(BlueprintReadOnly) bool bDisabled = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVehicleComponentDisabled, EWMVehicleComponent, Component);

/**
 * Component-based vehicle damage: Engine, Wheels/Tracks, Power System, Sensors, Communications, Armor
 * and Crew Compartment each track independent integrity. Damaging one component degrades a specific
 * capability instead of just subtracting from one shared health pool.
 */
UCLASS(ClassGroup = (WarMachine), meta = (BlueprintSpawnableComponent))
class UWMVehicleDamageComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UWMVehicleDamageComponent();

	UPROPERTY(BlueprintReadOnly, Category = "VehicleDamage")
	TArray<FWMVehicleComponentState> ComponentStates;

	UPROPERTY(BlueprintAssignable, Category = "VehicleDamage")
	FOnVehicleComponentDisabled OnComponentDisabled;

	UFUNCTION(BlueprintCallable, Category = "VehicleDamage")
	void ApplyComponentDamage(EWMVehicleComponent Component, float Amount);

	UFUNCTION(BlueprintCallable, Category = "VehicleDamage")
	void RepairComponent(EWMVehicleComponent Component, float Amount);

	UFUNCTION(BlueprintPure, Category = "VehicleDamage")
	float GetComponentIntegrity(EWMVehicleComponent Component) const;

	UFUNCTION(BlueprintPure, Category = "VehicleDamage")
	bool IsComponentDisabled(EWMVehicleComponent Component) const;

	/** Aggregate multiplier the vehicle's movement/physics should apply, from Engine/Wheels state. */
	UFUNCTION(BlueprintPure, Category = "VehicleDamage")
	float GetMobilityMultiplier() const;

protected:
	virtual void BeginPlay() override;
};
