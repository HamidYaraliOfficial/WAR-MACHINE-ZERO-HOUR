#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WMConvoy.h"
#include "WMLogisticsSubsystem.generated.h"

class AWMSupplyDepot;

/** Coordinates supply routes: which depots exist, which convoys are active, and the road-network routes between them. */
UCLASS()
class UWMLogisticsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Logistics")
	TArray<TObjectPtr<AWMSupplyDepot>> RegisteredDepots;

	UPROPERTY(BlueprintReadOnly, Category = "Logistics")
	TArray<TObjectPtr<AWMConvoy>> ActiveConvoys;

	UFUNCTION(BlueprintCallable, Category = "Logistics")
	void RegisterDepot(AWMSupplyDepot* Depot);

	UFUNCTION(BlueprintCallable, Category = "Logistics")
	AWMConvoy* DispatchConvoy(AWMSupplyDepot* FromDepot, const TArray<FVector>& Route, float Ammo, float Fuel, float Medical, float Parts);

	UFUNCTION(BlueprintCallable, Category = "Logistics")
	void TickAllConvoys(float DeltaSeconds);
};
