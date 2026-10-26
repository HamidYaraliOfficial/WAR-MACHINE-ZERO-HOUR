#include "WMCampaignClockSubsystem.h"
#include "WarMachine.h"

void UWMCampaignClockSubsystem::ConfigureSchedule(float WindowLengthMinutes, float GapMinutes, float TimeScale)
{
	OperationWindowLengthMinutes = FMath::Max(1.0f, WindowLengthMinutes);
	GapBetweenWindowsMinutes = FMath::Max(0.0f, GapMinutes);
	MinutesPerRealSecond = FMath::Max(0.0f, TimeScale);
}

void UWMCampaignClockSubsystem::StartClock()
{
	bWindowOpen = true;
	ElapsedInPhaseMinutes = 0.0f;
	UE_LOG(LogWarMachine, Log, TEXT("Campaign Clock started: window=%.0fmin, gap=%.0fmin, scale=%.2f min/sec"),
		OperationWindowLengthMinutes, GapBetweenWindowsMinutes, MinutesPerRealSecond);
}

void UWMCampaignClockSubsystem::Tick(float DeltaSeconds)
{
	ElapsedInPhaseMinutes += DeltaSeconds * MinutesPerRealSecond;
	const float PhaseLength = bWindowOpen ? OperationWindowLengthMinutes : GapBetweenWindowsMinutes;

	if (ElapsedInPhaseMinutes >= PhaseLength)
	{
		ElapsedInPhaseMinutes -= PhaseLength;
		bWindowOpen = !bWindowOpen;
		UE_LOG(LogWarMachine, Log, TEXT("Campaign Clock: operation window is now %s"), bWindowOpen ? TEXT("OPEN") : TEXT("CLOSED"));
	}
}

float UWMCampaignClockSubsystem::GetMinutesUntilNextTransition() const
{
	const float PhaseLength = bWindowOpen ? OperationWindowLengthMinutes : GapBetweenWindowsMinutes;
	return FMath::Max(0.0f, PhaseLength - ElapsedInPhaseMinutes);
}
