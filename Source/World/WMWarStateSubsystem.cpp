#include "WMWarStateSubsystem.h"
#include "WarMachine.h"

void UWMWarStateSubsystem::InitializeWarState()
{
	Indices = FWMWarIndices();
	UE_LOG(LogWarMachine, Log, TEXT("War State initialized. Overall score=%.2f"), GetOverallWarScore());
	OnWarIndicesChanged.Broadcast(Indices);
}

void UWMWarStateSubsystem::ApplyMissionOutcome(bool bSuccess, float TerritoryDelta, float SupplyDelta, float IntelDelta)
{
	const float Sign = bSuccess ? 1.0f : -1.0f;
	Indices.TerritoryControl += Sign * TerritoryDelta;
	Indices.SupplyStrength += Sign * SupplyDelta;
	Indices.IntelligenceQuality += Sign * IntelDelta;
	Indices.Morale += bSuccess ? 0.03f : -0.05f;
	ClampAndBroadcast();
}

void UWMWarStateSubsystem::RecordObjectiveDecision(EWMObjectiveType Type, bool bWasSkipped)
{
	if (!bWasSkipped) return;

	// Skipping intel-bearing objectives specifically degrades future intelligence quality;
	// skipping logistics-adjacent objectives degrades supply. This keeps the consequence tied
	// to *what* was skipped, not a flat generic penalty.
	if (Type == EWMObjectiveType::Recon || Type == EWMObjectiveType::Search)
	{
		Indices.IntelligenceQuality -= 0.05f;
	}
	else if (Type == EWMObjectiveType::Escort || Type == EWMObjectiveType::ConvoyProtection)
	{
		Indices.SupplyStrength -= 0.05f;
	}
	else if (Type == EWMObjectiveType::Rescue)
	{
		Indices.Morale -= 0.04f;
	}
	ClampAndBroadcast();
}

float UWMWarStateSubsystem::GetOverallWarScore() const
{
	return (Indices.TerritoryControl + Indices.SupplyStrength + Indices.IntelligenceQuality +
		Indices.Morale + Indices.InfrastructureIntegrity + Indices.MilitaryStrength) / 6.0f;
}

void UWMWarStateSubsystem::ClampAndBroadcast()
{
	Indices.TerritoryControl = FMath::Clamp(Indices.TerritoryControl, 0.0f, 1.0f);
	Indices.SupplyStrength = FMath::Clamp(Indices.SupplyStrength, 0.0f, 1.0f);
	Indices.IntelligenceQuality = FMath::Clamp(Indices.IntelligenceQuality, 0.0f, 1.0f);
	Indices.Morale = FMath::Clamp(Indices.Morale, 0.0f, 1.0f);
	Indices.InfrastructureIntegrity = FMath::Clamp(Indices.InfrastructureIntegrity, 0.0f, 1.0f);
	Indices.MilitaryStrength = FMath::Clamp(Indices.MilitaryStrength, 0.0f, 1.0f);
	OnWarIndicesChanged.Broadcast(Indices);
}
