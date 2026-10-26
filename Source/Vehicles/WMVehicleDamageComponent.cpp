#include "WMVehicleDamageComponent.h"
#include "WarMachine.h"

UWMVehicleDamageComponent::UWMVehicleDamageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWMVehicleDamageComponent::BeginPlay()
{
	Super::BeginPlay();
	if (ComponentStates.Num() == 0)
	{
		const EWMVehicleComponent AllComponents[] = {
			EWMVehicleComponent::Engine, EWMVehicleComponent::Wheels, EWMVehicleComponent::PowerSystem,
			EWMVehicleComponent::Sensors, EWMVehicleComponent::Communications, EWMVehicleComponent::Armor,
			EWMVehicleComponent::CrewCompartment, EWMVehicleComponent::WeaponMount
		};
		for (EWMVehicleComponent C : AllComponents)
		{
			FWMVehicleComponentState State;
			State.Component = C;
			ComponentStates.Add(State);
		}
	}
}

void UWMVehicleDamageComponent::ApplyComponentDamage(EWMVehicleComponent Component, float Amount)
{
	for (FWMVehicleComponentState& State : ComponentStates)
	{
		if (State.Component == Component)
		{
			State.Integrity = FMath::Clamp(State.Integrity - Amount, 0.0f, 1.0f);
			if (State.Integrity <= 0.0f && !State.bDisabled)
			{
				State.bDisabled = true;
				OnComponentDisabled.Broadcast(Component);
				UE_LOG(LogWarMachine, Log, TEXT("Vehicle component %d disabled."), (int32)Component);
			}
			return;
		}
	}
}

void UWMVehicleDamageComponent::RepairComponent(EWMVehicleComponent Component, float Amount)
{
	for (FWMVehicleComponentState& State : ComponentStates)
	{
		if (State.Component == Component)
		{
			State.Integrity = FMath::Clamp(State.Integrity + Amount, 0.0f, 1.0f);
			if (State.Integrity > 0.0f) State.bDisabled = false;
			return;
		}
	}
}

float UWMVehicleDamageComponent::GetComponentIntegrity(EWMVehicleComponent Component) const
{
	for (const FWMVehicleComponentState& State : ComponentStates)
	{
		if (State.Component == Component) return State.Integrity;
	}
	return 1.0f;
}

bool UWMVehicleDamageComponent::IsComponentDisabled(EWMVehicleComponent Component) const
{
	for (const FWMVehicleComponentState& State : ComponentStates)
	{
		if (State.Component == Component) return State.bDisabled;
	}
	return false;
}

float UWMVehicleDamageComponent::GetMobilityMultiplier() const
{
	const float EngineFactor = GetComponentIntegrity(EWMVehicleComponent::Engine);
	const float WheelsFactor = GetComponentIntegrity(EWMVehicleComponent::Wheels);
	if (IsComponentDisabled(EWMVehicleComponent::Engine) || IsComponentDisabled(EWMVehicleComponent::Wheels)) return 0.0f;
	return FMath::Clamp(EngineFactor * 0.6f + WheelsFactor * 0.4f, 0.0f, 1.0f);
}
