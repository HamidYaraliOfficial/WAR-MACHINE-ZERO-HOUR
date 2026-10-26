#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WMCampaignClockSubsystem.generated.h"

/**
 * Configurable campaign scheduling clock: the player (or a difficulty preset) sets how long an
 * "Operation Window" (the period the Command Center accepts a new mission launch) stays open and
 * how long the cooldown/travel/prep gap to the next window is. The HUD/Base UI can always query
 * "is a window open right now" and "how long until the next one" -- both fully data-driven from
 * values the user enters, not hardcoded.
 */
UCLASS()
class UWMCampaignClockSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	/** Minutes (in-game) that an operation window stays open once it begins. User-configurable. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CampaignClock")
	float OperationWindowLengthMinutes = 180.0f;

	/** Minutes (in-game) between the close of one window and the opening of the next. User-configurable. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CampaignClock")
	float GapBetweenWindowsMinutes = 360.0f;

	/** How many in-game minutes pass per real-time second while the clock is running. User-configurable. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CampaignClock")
	float MinutesPerRealSecond = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "CampaignClock")
	void ConfigureSchedule(float WindowLengthMinutes, float GapMinutes, float TimeScale);

	UFUNCTION(BlueprintCallable, Category = "CampaignClock")
	void StartClock();

	UFUNCTION(BlueprintCallable, Category = "CampaignClock")
	void Tick(float DeltaSeconds);

	UFUNCTION(BlueprintPure, Category = "CampaignClock")
	bool IsOperationWindowOpen() const { return bWindowOpen; }

	/** Minutes remaining until the current window closes, or until the next one opens if currently closed. */
	UFUNCTION(BlueprintPure, Category = "CampaignClock")
	float GetMinutesUntilNextTransition() const;

private:
	bool bWindowOpen = true;
	float ElapsedInPhaseMinutes = 0.0f;
};
