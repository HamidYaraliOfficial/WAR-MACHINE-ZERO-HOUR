#include "WMRadioSubsystem.h"

void UWMRadioSubsystem::PostMessage(FName SpeakerCallsign, EWMRadioEventType EventType, FText Text)
{
	FWMRadioMessage Message;
	Message.SpeakerCallsign = SpeakerCallsign;
	Message.EventType = EventType;
	Message.Text = Text;
	Message.SignalQuality = NetworkSignalQuality;
	OnRadioMessage.Broadcast(Message);
}
