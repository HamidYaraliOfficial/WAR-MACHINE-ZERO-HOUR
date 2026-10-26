#include "WMReplaySubsystem.h"

void UWMReplaySubsystem::StartRecording()
{
	Timeline.Empty();
	bIsRecording = true;
}

void UWMReplaySubsystem::StopRecording()
{
	bIsRecording = false;
}

void UWMReplaySubsystem::RecordEvent(FName EventTag, FVector Location, const FString& Description)
{
	if (!bIsRecording) return;
	FWMReplayEvent Event;
	Event.TimestampSeconds = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
	Event.EventTag = EventTag;
	Event.Location = Location;
	Event.Description = Description;
	Timeline.Add(Event);
}

TArray<FWMReplayEvent> UWMReplaySubsystem::GetEventsBetween(float StartSeconds, float EndSeconds) const
{
	return Timeline.FilterByPredicate([StartSeconds, EndSeconds](const FWMReplayEvent& E) {
		return E.TimestampSeconds >= StartSeconds && E.TimestampSeconds <= EndSeconds;
	});
}
