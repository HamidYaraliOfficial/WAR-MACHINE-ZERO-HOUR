#include "WMMedicalComponent.h"
#include "WarMachine.h"

UWMMedicalComponent::UWMMedicalComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWMMedicalComponent::GoDown()
{
	if (UnitState == EWMUnitState::KIA) return;
	UnitState = EWMUnitState::Downed;
	BleedOutRemaining = BleedOutSeconds;
	OnUnitDowned.Broadcast(GetOwner());
}

void UWMMedicalComponent::TickBleedOut(float DeltaSeconds)
{
	if (UnitState != EWMUnitState::Downed) return;
	BleedOutRemaining = FMath::Max(0.0f, BleedOutRemaining - DeltaSeconds);
	if (BleedOutRemaining <= 0.0f)
	{
		UnitState = EWMUnitState::KIA;
		UE_LOG(LogWarMachine, Log, TEXT("%s could not be stabilized in time."), *GetNameSafe(GetOwner()));
	}
}

bool UWMMedicalComponent::StabilizeWithSupplies()
{
	if (UnitState != EWMUnitState::Downed || MedicalSuppliesCarried <= 0) return false;
	--MedicalSuppliesCarried;
	UnitState = EWMUnitState::Wounded;
	OnUnitRevived.Broadcast(GetOwner());
	return true;
}

bool UWMMedicalComponent::StabilizeByMedic(UWMMedicalComponent* Medic)
{
	if (!Medic || Medic->MedicalSuppliesCarried <= 0 || UnitState != EWMUnitState::Downed) return false;
	--Medic->MedicalSuppliesCarried;
	UnitState = EWMUnitState::Wounded;
	OnUnitRevived.Broadcast(GetOwner());
	return true;
}
