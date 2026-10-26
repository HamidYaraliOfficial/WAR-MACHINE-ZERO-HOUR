#include "WMWeatherSubsystem.h"

void UWMWeatherSubsystem::AdvanceTime(float DeltaSeconds, float MinutesPerRealSecond)
{
	TimeOfDayHours += (DeltaSeconds * MinutesPerRealSecond) / 60.0f;
	if (TimeOfDayHours >= 24.0f) TimeOfDayHours -= 24.0f;
}

void UWMWeatherSubsystem::SetWeather(EWMWeatherType NewWeather)
{
	if (CurrentWeather != NewWeather)
	{
		CurrentWeather = NewWeather;
		OnWeatherChanged.Broadcast(CurrentWeather);
	}
}

float UWMWeatherSubsystem::GetVisibilityModifier() const
{
	float Base = IsNight() ? 0.5f : 1.0f;
	switch (CurrentWeather)
	{
		case EWMWeatherType::Fog:   return Base * 0.35f;
		case EWMWeatherType::Storm: return Base * 0.45f;
		case EWMWeatherType::Rain:  return Base * 0.7f;
		case EWMWeatherType::Dust:  return Base * 0.5f;
		case EWMWeatherType::Snow:  return Base * 0.6f;
		default:                    return Base;
	}
}

float UWMWeatherSubsystem::GetVehicleMobilityModifier() const
{
	switch (CurrentWeather)
	{
		case EWMWeatherType::Storm: return 0.55f;
		case EWMWeatherType::Rain:  return 0.8f;
		case EWMWeatherType::Snow:  return 0.65f;
		case EWMWeatherType::Dust:  return 0.85f;
		default:                    return 1.0f;
	}
}

float UWMWeatherSubsystem::GetSensorRangeModifier() const
{
	switch (CurrentWeather)
	{
		case EWMWeatherType::Fog:   return 0.4f;
		case EWMWeatherType::Storm: return 0.5f;
		case EWMWeatherType::Dust:  return 0.6f;
		default:                    return 1.0f;
	}
}
