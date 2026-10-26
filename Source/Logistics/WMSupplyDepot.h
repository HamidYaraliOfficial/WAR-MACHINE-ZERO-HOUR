#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WMSupplyDepot.generated.h"

USTRUCT(BlueprintType)
struct FWMSupplyStock
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadOnly) float Ammunition = 100.0f;
	UPROPERTY(BlueprintReadOnly) float FuelOrEnergy = 100.0f;
	UPROPERTY(BlueprintReadOnly) float MedicalSupplies = 100.0f;
	UPROPERTY(BlueprintReadOnly) float RepairParts = 100.0f;
};

/** A world location that stores supplies and can dispatch a Convoy toward a requesting unit. */
UCLASS()
class AWMSupplyDepot : public AActor
{
	GENERATED_BODY()
public:
	AWMSupplyDepot();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Logistics")
	FWMSupplyStock Stock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Logistics")
	FName DepotId;

	UFUNCTION(BlueprintCallable, Category = "Logistics")
	bool WithdrawSupplies(float Ammo, float Fuel, float Medical, float Parts);

	UFUNCTION(BlueprintCallable, Category = "Logistics")
	void Replenish(float Ammo, float Fuel, float Medical, float Parts);
};
