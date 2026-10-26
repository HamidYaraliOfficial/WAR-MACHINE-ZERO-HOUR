#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WMTypes.h"
#include "WMWeaponData.h"
#include "WMWeaponBase.generated.h"

/**
 * Runtime weapon actor. Damage/impact resolution goes through the abstracted gameplay ballistics
 * helper (Distance/Angle/Material/WeaponClass only) -- this is a game-feel model, not a real-world
 * exterior/terminal ballistics simulation, and it is not intended or suitable for that purpose.
 */
UCLASS(BlueprintType, Blueprintable)
class AWMWeaponBase : public AActor
{
	GENERATED_BODY()
public:
	AWMWeaponBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UWMWeaponDataAsset> WeaponData;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	int32 CurrentMagazine = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	EWMFireMode CurrentFireMode = EWMFireMode::Semi;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	float ConditionState = 1.0f; // 0..1 fouling/wear -- affects reliability, not lethality escalation

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	TArray<FName> EquippedAttachmentIds;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool TryFire(float& OutEffectiveAccuracy, float& OutEffectiveRecoil);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EquipAttachment(FName AttachmentId);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void CycleFireMode();

	/** Simplified, game-only hit resolution: distance falloff + angle + material factor. Not a real-world model. */
	UFUNCTION(BlueprintPure, Category = "Weapon")
	float ComputeGameplayDamage(float DistanceMeters, float ImpactAngleDegrees, float MaterialResistance01) const;

private:
	float GetAttachmentSum(float FWMWeaponAttachment::* Field) const;
};
