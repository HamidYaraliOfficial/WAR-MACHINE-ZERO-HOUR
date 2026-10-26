#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WMTypes.h"
#include "WMSquadMember.generated.h"

/** A single soldier's runtime gameplay state, independent of its visual Pawn representation. */
UCLASS(BlueprintType)
class UWMSquadMember : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Identity")
	FGuid MemberId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
	EWMUnitRole Role = EWMUnitRole::Infantry;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	EWMUnitState UnitState = EWMUnitState::Healthy;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	FWMGaugeValue Health;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	FWMGaugeValue Morale;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	FWMGaugeValue Fatigue;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	FWMGaugeValue Confidence;

	UPROPERTY(BlueprintReadWrite, Category = "State")
	FWMGaugeValue Discipline;

	/** Experience accrued from completed missions, spent on Traits/Perks. */
	UPROPERTY(BlueprintReadOnly, Category = "Progression")
	int32 Experience = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Progression")
	TArray<FName> UnlockedTraits;

	UFUNCTION(BlueprintCallable, Category = "State")
	void ApplyDamage(float Amount, EWMDamageType DamageType);

	UFUNCTION(BlueprintCallable, Category = "State")
	void ApplySuppression(float Intensity);

	UFUNCTION(BlueprintCallable, Category = "Progression")
	void GrantExperience(int32 Amount);

	UFUNCTION(BlueprintPure, Category = "State")
	bool IsCombatEffective() const { return UnitState == EWMUnitState::Healthy || UnitState == EWMUnitState::Wounded; }
};
