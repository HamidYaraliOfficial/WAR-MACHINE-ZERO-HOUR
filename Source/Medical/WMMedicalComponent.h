#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WMTypes.h"
#include "WMMedicalComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitDowned, AActor*, DownedActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitRevived, AActor*, RevivedActor);

/**
 * Lightweight, non-graphic medical state machine: Health / Injury / Downed / Recovery only, no gore.
 * A downed unit must be stabilized by a Medic (or self-administered supplies) within a bleed-out window.
 */
UCLASS(ClassGroup = (WarMachine), meta = (BlueprintSpawnableComponent))
class UWMMedicalComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UWMMedicalComponent();

	UPROPERTY(BlueprintReadOnly, Category = "Medical") EWMUnitState UnitState = EWMUnitState::Healthy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Medical") float BleedOutSeconds = 90.0f;
	UPROPERTY(BlueprintReadOnly, Category = "Medical") float BleedOutRemaining = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Medical") int32 MedicalSuppliesCarried = 2;

	UPROPERTY(BlueprintAssignable, Category = "Medical") FOnUnitDowned OnUnitDowned;
	UPROPERTY(BlueprintAssignable, Category = "Medical") FOnUnitRevived OnUnitRevived;

	UFUNCTION(BlueprintCallable, Category = "Medical") void GoDown();
	UFUNCTION(BlueprintCallable, Category = "Medical") void TickBleedOut(float DeltaSeconds);
	UFUNCTION(BlueprintCallable, Category = "Medical") bool StabilizeWithSupplies();
	UFUNCTION(BlueprintCallable, Category = "Medical") bool StabilizeByMedic(UWMMedicalComponent* Medic);
};
