#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WMTypes.h"
#include "WMWeatherSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeatherChanged, EWMWeatherType, NewWeather);

/** Drives current weather + time-of-day and exposes gameplay-relevant modifiers (visibility, mobility, sensors). */
UCLASS()
class UWMWeatherSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Weather")
	EWMWeatherType CurrentWeather = EWMWeatherType::Clear;

	UPROPERTY(BlueprintReadOnly, Category = "Weather")
	float TimeOfDayHours = 6.0f; // 0..24

	UPROPERTY(BlueprintAssignable, Category = "Weather")
	FOnWeatherChanged OnWeatherChanged;

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void AdvanceTime(float DeltaSeconds, float MinutesPerRealSecond);

	UFUNCTION(BlueprintCallable, Category = "Weather")
	void SetWeather(EWMWeatherType NewWeather);

	UFUNCTION(BlueprintPure, Category = "Weather")
	float GetVisibilityModifier() const;

	UFUNCTION(BlueprintPure, Category = "Weather")
	float GetVehicleMobilityModifier() const;

	UFUNCTION(BlueprintPure, Category = "Weather")
	float GetSensorRangeModifier() const;

	UFUNCTION(BlueprintPure, Category = "Weather")
	bool IsNight() const { return TimeOfDayHours < 6.0f || TimeOfDayHours > 20.0f; }
};
