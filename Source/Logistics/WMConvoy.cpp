#include "WMConvoy.h"
#include "WarMachine.h"

AWMConvoy::AWMConvoy()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWMConvoy::SetState(EWMConvoyState NewState)
{
	State = NewState;
	OnConvoyStateChanged.Broadcast(State);
}

void AWMConvoy::BeginRoute(const TArray<FVector>& Route)
{
	RoutePoints = Route;
	CurrentRouteIndex = 0;
	SetState(EWMConvoyState::EnRoute);
}

void AWMConvoy::TickMovement(float DeltaSeconds)
{
	if (State != EWMConvoyState::EnRoute || RoutePoints.Num() == 0 || CurrentRouteIndex >= RoutePoints.Num())
	{
		return;
	}

	const FVector Target = RoutePoints[CurrentRouteIndex];
	const FVector CurrentLocation = GetActorLocation();
	const FVector ToTarget = Target - CurrentLocation;
	const float DistanceRemaining = ToTarget.Size();
	const float StepDistance = SpeedMetersPerSecond * 100.0f * DeltaSeconds; // cm/s

	if (StepDistance >= DistanceRemaining)
	{
		SetActorLocation(Target);
		++CurrentRouteIndex;
		if (CurrentRouteIndex >= RoutePoints.Num())
		{
			SetState(EWMConvoyState::Delivered);
			UE_LOG(LogWMLogistics, Log, TEXT("Convoy %s delivered cargo."), *GetName());
		}
	}
	else
	{
		SetActorLocation(CurrentLocation + ToTarget.GetSafeNormal() * StepDistance);
	}
}

void AWMConvoy::ApplyAttackDamage(float CargoLossFraction)
{
	CargoLossFraction = FMath::Clamp(CargoLossFraction, 0.0f, 1.0f);
	Cargo.Ammunition *= (1.0f - CargoLossFraction);
	Cargo.FuelOrEnergy *= (1.0f - CargoLossFraction);
	Cargo.MedicalSupplies *= (1.0f - CargoLossFraction);
	Cargo.RepairParts *= (1.0f - CargoLossFraction);

	SetState(CargoLossFraction >= 0.95f ? EWMConvoyState::Destroyed : EWMConvoyState::UnderAttack);
}

void AWMConvoy::RerouteAround(FVector DetourPoint)
{
	if (CurrentRouteIndex < RoutePoints.Num())
	{
		RoutePoints.Insert(DetourPoint, CurrentRouteIndex);
	}
	SetState(EWMConvoyState::Rerouted);
}
