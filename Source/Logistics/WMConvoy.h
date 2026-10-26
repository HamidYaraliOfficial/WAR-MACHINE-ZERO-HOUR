#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WMSupplyDepot.h"
#include "WMConvoy.generated.h"

UENUM(BlueprintType)
enum class EWMConvoyState : uint8
{
	Staging, EnRoute, Delayed, UnderAttack, Delivered, Destroyed, Rerouted
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConvoyStateChanged, EWMConvoyState, NewState);

/** A moving logistics convoy that can be delayed, damaged, or forced to reroute -- it is a simulated actor, not a teleporting resource transfer. */
UCLASS()
class AWMConvoy : public AActor
{
	GENERATED_BODY()
public:
	AWMConvoy();

	UPROPERTY(BlueprintReadOnly, Category = "Logistics")
	EWMConvoyState State = EWMConvoyState::Staging;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Logistics")
	FWMSupplyStock Cargo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Logistics")
	TArray<FVector> RoutePoints;

	UPROPERTY(BlueprintReadOnly, Category = "Logistics")
	int32 CurrentRouteIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Logistics")
	float SpeedMetersPerSecond = 8.0f;

	UPROPERTY(BlueprintAssignable, Category = "Logistics")
	FOnConvoyStateChanged OnConvoyStateChanged;

	UFUNCTION(BlueprintCallable, Category = "Logistics")
	void BeginRoute(const TArray<FVector>& Route);

	UFUNCTION(BlueprintCallable, Category = "Logistics")
	void TickMovement(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "Logistics")
	void ApplyAttackDamage(float CargoLossFraction);

	UFUNCTION(BlueprintCallable, Category = "Logistics")
	void RerouteAround(FVector DetourPoint);

private:
	void SetState(EWMConvoyState NewState);
};
