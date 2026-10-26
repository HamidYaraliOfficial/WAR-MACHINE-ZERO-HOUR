#include "WMLogisticsSubsystem.h"
#include "WMSupplyDepot.h"
#include "WarMachine.h"

void UWMLogisticsSubsystem::RegisterDepot(AWMSupplyDepot* Depot)
{
	if (Depot) RegisteredDepots.AddUnique(Depot);
}

AWMConvoy* UWMLogisticsSubsystem::DispatchConvoy(AWMSupplyDepot* FromDepot, const TArray<FVector>& Route, float Ammo, float Fuel, float Medical, float Parts)
{
	if (!FromDepot || !FromDepot->WithdrawSupplies(Ammo, Fuel, Medical, Parts))
	{
		UE_LOG(LogWMLogistics, Warning, TEXT("Convoy dispatch failed: depot %s lacks requested supplies."),
			FromDepot ? *FromDepot->DepotId.ToString() : TEXT("<null>"));
		return nullptr;
	}

	AWMConvoy* Convoy = GetWorld()->SpawnActor<AWMConvoy>();
	if (!Convoy) return nullptr;

	Convoy->Cargo.Ammunition = Ammo;
	Convoy->Cargo.FuelOrEnergy = Fuel;
	Convoy->Cargo.MedicalSupplies = Medical;
	Convoy->Cargo.RepairParts = Parts;
	Convoy->BeginRoute(Route);
	ActiveConvoys.Add(Convoy);
	return Convoy;
}

void UWMLogisticsSubsystem::TickAllConvoys(float DeltaSeconds)
{
	for (AWMConvoy* Convoy : ActiveConvoys)
	{
		if (Convoy) Convoy->TickMovement(DeltaSeconds);
	}
	ActiveConvoys.RemoveAll([](const AWMConvoy* C) {
		return !C || C->State == EWMConvoyState::Delivered || C->State == EWMConvoyState::Destroyed;
	});
}
