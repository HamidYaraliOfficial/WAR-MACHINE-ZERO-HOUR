#pragma once
#include "CoreMinimal.h"
#include "WMTypes.generated.h"

UENUM(BlueprintType)
enum class EWMFaction : uint8
{
	None			UMETA(DisplayName = "None"),
	IronCompact		UMETA(DisplayName = "Iron Compact"),      // Fictional mechanized coalition
	RedMeridian		UMETA(DisplayName = "Red Meridian"),       // Fictional insurgent/irregular force
	NorthGate		UMETA(DisplayName = "Northgate Directorate"), // Fictional peer conventional army
	FreeCompanies	UMETA(DisplayName = "Free Companies")      // Fictional private military contractors
};

UENUM(BlueprintType)
enum class EWMUnitRole : uint8
{
	Infantry, Recon, Mechanized, Armored, Support, Engineer, Medical, Drone, Transport, FireSupport
};

UENUM(BlueprintType)
enum class EWMObjectiveType : uint8
{
	Assault, Secure, Defend, Recon, Rescue, Escort, Recovery, Sabotage,
	Search, HoldPosition, Evacuate, ConvoyProtection, Extraction
};

UENUM(BlueprintType)
enum class EWMObjectiveState : uint8
{
	Locked, Available, Active, Completed, Failed, Substituted, Skipped
};

UENUM(BlueprintType)
enum class EWMFormation : uint8
{
	Wedge, File, Line, Diamond, Staggered, OnMe
};

UENUM(BlueprintType)
enum class EWMRulesOfEngagement : uint8
{
	HoldFire, ReturnFireOnly, EngageAtWill, WeaponsFree
};

UENUM(BlueprintType)
enum class EWMOrderType : uint8
{
	Move, Hold, Follow, Assault, Suppress, Flank, Regroup, TakeCover, Retreat, MountVehicle, DismountVehicle
};

UENUM(BlueprintType)
enum class EWMUnitState : uint8
{
	Healthy, Wounded, Downed, KIA, Suppressed, Fleeing
};

UENUM(BlueprintType)
enum class EWMDamageType : uint8
{
	Kinetic, Explosive, Fire, Environmental, Fall
};

UENUM(BlueprintType)
enum class EWMVehicleComponent : uint8
{
	Engine, Wheels, PowerSystem, Sensors, Communications, Armor, CrewCompartment, WeaponMount
};

UENUM(BlueprintType)
enum class EWMWeatherType : uint8
{
	Clear, Rain, Storm, Fog, Dust, Snow
};

UENUM(BlueprintType)
enum class EWMDifficulty : uint8
{
	Recruit, Regular, Veteran, Hardcore, Custom
};

UENUM(BlueprintType)
enum class EWMAIAwarenessLevel : uint8
{
	Unaware, Suspicious, Investigating, Alert, Engaged
};

UENUM(BlueprintType)
enum class EWMSupportRequestType : uint8
{
	ReconSweep, SupplyDrop, VehicleSupport, MedicalEvac, Reinforcement, FireMission
};

/** Simple 0..1 clamp helper struct used across gameplay meters (morale, fatigue, awareness, etc). */
USTRUCT(BlueprintType)
struct FWMGaugeValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gauge")
	float Value = 1.0f;

	void Add(float Delta) { Value = FMath::Clamp(Value + Delta, 0.0f, 1.0f); }
	void Set(float NewValue) { Value = FMath::Clamp(NewValue, 0.0f, 1.0f); }
};
