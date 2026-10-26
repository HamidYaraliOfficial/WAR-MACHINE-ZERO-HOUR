#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WMTypes.h"
#include "WMWeaponData.generated.h"

UENUM(BlueprintType)
enum class EWMWeaponClass : uint8
{
	Rifle, SMG, Shotgun, SupportWeapon, Sidearm, Launcher
};

UENUM(BlueprintType)
enum class EWMFireMode : uint8
{
	Semi, Burst, Full
};

/** A single fictional attachment; every attachment carries a real gameplay trade-off, never a pure upgrade. */
USTRUCT(BlueprintType)
struct FWMWeaponAttachment
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName AttachmentId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float AccuracyModifier = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float RecoilModifier = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float HandlingModifier = 0.0f;   // affects ADS/reload speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly) float RangeModifier = 0.0f;
};

/**
 * Entirely fictional, gameplay-abstracted weapon statistics. Names/specs are invented for this game and
 * are not derived from, or usable as, real-world firearm manufacturing or ballistic reference data.
 */
UCLASS(BlueprintType)
class UWMWeaponDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon") FText DisplayName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon") EWMWeaponClass WeaponClass = EWMWeaponClass::Rifle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon") TArray<EWMFireMode> SupportedFireModes;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon") int32 MagazineSize = 30;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon") float ReloadTimeSeconds = 2.4f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon") float BaseAccuracy = 0.8f;      // 0..1 gameplay abstraction
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon") float BaseRecoil = 0.3f;         // 0..1 gameplay abstraction
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon") float EffectiveRangeMeters = 300.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon") float BaseDamage = 28.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon") TArray<FWMWeaponAttachment> AvailableAttachments;
};
