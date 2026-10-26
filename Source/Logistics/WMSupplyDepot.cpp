#include "WMSupplyDepot.h"

AWMSupplyDepot::AWMSupplyDepot()
{
	PrimaryActorTick.bCanEverTick = false;
	DepotId = FName(*FGuid::NewGuid().ToString());
}

bool AWMSupplyDepot::WithdrawSupplies(float Ammo, float Fuel, float Medical, float Parts)
{
	if (Stock.Ammunition < Ammo || Stock.FuelOrEnergy < Fuel || Stock.MedicalSupplies < Medical || Stock.RepairParts < Parts)
	{
		return false;
	}
	Stock.Ammunition -= Ammo;
	Stock.FuelOrEnergy -= Fuel;
	Stock.MedicalSupplies -= Medical;
	Stock.RepairParts -= Parts;
	return true;
}

void AWMSupplyDepot::Replenish(float Ammo, float Fuel, float Medical, float Parts)
{
	Stock.Ammunition += Ammo;
	Stock.FuelOrEnergy += Fuel;
	Stock.MedicalSupplies += Medical;
	Stock.RepairParts += Parts;
}
