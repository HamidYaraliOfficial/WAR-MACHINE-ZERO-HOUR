#include "WMDroneComponent.h"
#include "WarMachine.h"

UWMDroneComponent::UWMDroneComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	CurrentBattery = MaxBattery;
}

void UWMDroneComponent::Launch()
{
	if (!CanOperate())
	{
		UE_LOG(LogWarMachine, Warning, TEXT("Drone cannot launch: battery=%.1f damage=%.2f"), CurrentBattery, DamageState);
		return;
	}
	FlightMode = EWMDroneFlightMode::Cruising;
}

void UWMDroneComponent::TickFlight(float DeltaSeconds, FVector CurrentLocation, float SignalObstruction01)
{
	if (FlightMode == EWMDroneFlightMode::Grounded) return;

	CurrentBattery = FMath::Max(0.0f, CurrentBattery - DeltaSeconds * 1.2f);
	const float DistanceFromHome = FVector::Dist(CurrentLocation, HomeLocation);
	SignalQuality = FMath::Clamp(1.0f - (DistanceFromHome / FMath::Max(1.0f, MaxRangeMeters * 100.0f)) - SignalObstruction01, 0.0f, 1.0f);

	if (CurrentBattery <= 15.0f && FlightMode != EWMDroneFlightMode::Returning)
	{
		FlightMode = EWMDroneFlightMode::Returning;
		UE_LOG(LogWarMachine, Log, TEXT("Drone battery low (%.1f%%), returning to home."), CurrentBattery);
	}
	if (CurrentBattery <= 0.0f)
	{
		FlightMode = EWMDroneFlightMode::Grounded;
	}
}

void UWMDroneComponent::ApplyDamage(float Amount)
{
	DamageState = FMath::Clamp(DamageState + Amount, 0.0f, 1.0f);
	if (DamageState >= 1.0f)
	{
		FlightMode = EWMDroneFlightMode::Grounded;
		UE_LOG(LogWarMachine, Log, TEXT("Drone destroyed."));
	}
}
