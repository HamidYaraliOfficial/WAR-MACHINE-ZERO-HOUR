#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WMReplaySubsystem.generated.h"

USTRUCT(BlueprintType)
struct FWMReplayEvent
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly) float TimestampSeconds = 0.0f;
	UPROPERTY(BlueprintReadOnly) FName EventTag;
	UPROPERTY(BlueprintReadOnly) FVector Location = FVector::ZeroVector;
	UPROPERTY(BlueprintReadOnly) FString Description;
};

/**
 * Records a lightweight event timeline (orders, objective changes, contacts, vehicle losses,
 * reinforcement, extraction) during a mission so it can be scrubbed afterward on the Tactical
 * Replay Map / Cinematic Replay Camera without needing full frame-by-frame state capture.
 */
UCLASS()
class UWMReplaySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Replay")
	TArray<FWMReplayEvent> Timeline;

	UPROPERTY(BlueprintReadOnly, Category = "Replay")
	bool bIsRecording = false;

	UFUNCTION(BlueprintCallable, Category = "Replay") void StartRecording();
	UFUNCTION(BlueprintCallable, Category = "Replay") void StopRecording();
	UFUNCTION(BlueprintCallable, Category = "Replay") void RecordEvent(FName EventTag, FVector Location, const FString& Description);
	UFUNCTION(BlueprintPure, Category = "Replay") TArray<FWMReplayEvent> GetEventsBetween(float StartSeconds, float EndSeconds) const;
};
