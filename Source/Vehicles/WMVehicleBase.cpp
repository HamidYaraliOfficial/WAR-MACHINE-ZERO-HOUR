#include "WMVehicleBase.h"
#include "WMVehicleDamageComponent.h"

AWMVehicleBase::AWMVehicleBase()
{
	PrimaryActorTick.bCanEverTick = true;
	DamageComponent = CreateDefaultSubobject<UWMVehicleDamageComponent>(TEXT("DamageComponent"));
	CurrentFuelOrEnergy = MaxFuelOrEnergy;
}

bool AWMVehicleBase::AssignCrew(APawn* Crewman)
{
	if (!Crewman || OccupyingCrew.Num() >= MaxCrewSeats) return false;
	OccupyingCrew.AddUnique(Crewman);
	return true;
}

bool AWMVehicleBase::IsOperational() const
{
	if (CurrentFuelOrEnergy <= 0.0f) return false;
	if (DamageComponent && DamageComponent->IsComponentDisabled(EWMVehicleComponent::Engine)) return false;
	return true;
}
