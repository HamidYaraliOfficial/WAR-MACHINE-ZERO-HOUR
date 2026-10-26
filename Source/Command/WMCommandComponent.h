#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WMTypes.h"
#include "WMCommandComponent.generated.h"

class UWMSquadComponent;

USTRUCT(BlueprintType)
struct FWMCommandResource
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) EWMSupportRequestType Type = EWMSupportRequestType::ReconSweep;
	UPROPERTY(BlueprintReadOnly) int32 ChargesAvailable = 1;
	UPROPERTY(BlueprintReadOnly) float CooldownRemainingSeconds = 0.0f;
};

/** Player-facing command authority: the Command Wheel / Tactical Map talk to this component. */
UCLASS(ClassGroup = (WarMachine), meta = (BlueprintSpawnableComponent))
class UWMCommandComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UWMCommandComponent();

	UPROPERTY(BlueprintReadOnly, Category = "Command")
	TArray<TObjectPtr<UWMSquadComponent>> CommandedSquads;

	UPROPERTY(BlueprintReadOnly, Category = "Command")
	TArray<FWMCommandResource> Resources;

	UFUNCTION(BlueprintCallable, Category = "Command")
	void RegisterSquad(UWMSquadComponent* Squad);

	UFUNCTION(BlueprintCallable, Category = "Command")
	void IssueOrderToSquad(FName SquadId, EWMOrderType Order, FVector TargetLocation, FName FireTeamId);

	/** Attempts to spend a limited Command Resource (drone sweep, vehicle support, med-evac, ...). */
	UFUNCTION(BlueprintCallable, Category = "Command")
	bool RequestSupport(EWMSupportRequestType Type);

	UFUNCTION(BlueprintCallable, Category = "Command")
	void TickCooldowns(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "Command")
	void ConfigureResource(EWMSupportRequestType Type, int32 Charges, float CooldownSeconds);

private:
	TMap<EWMSupportRequestType, float> ResourceCooldownDurations;
};
