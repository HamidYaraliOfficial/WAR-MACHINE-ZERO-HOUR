#include "WMWorldEventEngine.h"
#include "WarMachine.h"

void UWMWorldEventEngine::EvaluateTick(float SupplyStrength, float InfrastructureIntegrity, float IntelligenceQuality, float DeltaSeconds)
{
	AccumulatedSeconds += DeltaSeconds;
	if (AccumulatedSeconds < 30.0f) return; // evaluate roughly every 30 seconds of sim time
	AccumulatedSeconds = 0.0f;

	// Weighted odds derived from world state -- low state values raise the chance of the matching negative event.
	const float ConvoyDelayOdds = FMath::Clamp(1.0f - SupplyStrength, 0.05f, 0.8f);
	const float CommsFailureOdds = FMath::Clamp(1.0f - InfrastructureIntegrity, 0.05f, 0.7f);
	const float ReconDiscoveryOdds = FMath::Clamp(IntelligenceQuality * 0.6f, 0.05f, 0.6f);

	const float Roll = FMath::FRand();
	if (Roll < ConvoyDelayOdds * 0.33f)
	{
		FireEvent(EWMWorldEventType::ConvoyDelay, FVector::ZeroVector);
	}
	else if (Roll < ConvoyDelayOdds * 0.33f + CommsFailureOdds * 0.33f)
	{
		FireEvent(EWMWorldEventType::CommunicationsFailure, FVector::ZeroVector);
	}
	else if (Roll < ConvoyDelayOdds * 0.33f + CommsFailureOdds * 0.33f + ReconDiscoveryOdds * 0.33f)
	{
		FireEvent(EWMWorldEventType::ReconDiscovery, FVector::ZeroVector);
	}
}

void UWMWorldEventEngine::FireEvent(EWMWorldEventType Type, FVector Location)
{
	FWMWorldEvent Event;
	Event.EventType = Type;
	Event.Location = Location;
	Event.TimestampSeconds = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	OnWorldEventTriggered.Broadcast(Event);
	UE_LOG(LogWarMachine, Log, TEXT("World Event triggered: %d"), (int32)Type);
}
