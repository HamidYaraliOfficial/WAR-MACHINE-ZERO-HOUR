#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WMRadioSubsystem.generated.h"

UENUM(BlueprintType)
enum class EWMRadioEventType : uint8
{
	Contact, ObjectiveChange, Reinforcement, SupplyStatus, Extraction, General
};

USTRUCT(BlueprintType)
struct FWMRadioMessage
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly) FName SpeakerCallsign;
	UPROPERTY(BlueprintReadOnly) EWMRadioEventType EventType = EWMRadioEventType::General;
	UPROPERTY(BlueprintReadOnly) FText Text;
	UPROPERTY(BlueprintReadOnly) float SignalQuality = 1.0f; // 0..1, affects whether it's garbled in UI/audio
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRadioMessage, const FWMRadioMessage&, Message);

/**
 * Data-driven radio/comms bus: any gameplay system (Mission Director, Logistics, Command) posts an
 * event here and it is rendered as chatter + subtitle, gated by the current network's signal quality.
 */
UCLASS()
class UWMRadioSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Radio") float NetworkSignalQuality = 1.0f;

	UPROPERTY(BlueprintAssignable, Category = "Radio") FOnRadioMessage OnRadioMessage;

	UFUNCTION(BlueprintCallable, Category = "Radio")
	void PostMessage(FName SpeakerCallsign, EWMRadioEventType EventType, FText Text);

	UFUNCTION(BlueprintCallable, Category = "Radio")
	void SetNetworkSignalQuality(float Quality) { NetworkSignalQuality = FMath::Clamp(Quality, 0.0f, 1.0f); }
};
