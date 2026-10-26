#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WMTypes.h"
#include "WMUtilityAIComponent.generated.h"

/** One candidate behavior scored by the utility function. Chosen behavior drives which Behavior Tree subtree runs. */
USTRUCT(BlueprintType)
struct FWMUtilityAction
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) FName ActionTag;
	UPROPERTY(BlueprintReadOnly) float Score = 0.0f;
};

/**
 * Utility-AI layer that sits above the Behavior Tree: scores candidate actions (TakeCover, Suppress, Flank,
 * Retreat, Regroup, Advance, CallForMedic) from perceived threat, morale/fatigue/confidence/discipline and
 * squad cohesion, then pushes the winner into the Behavior Tree Blackboard as "CurrentUtilityAction".
 * The AI only reasons over information it has actually perceived (see WMPerceptionComponent) -- it never
 * reads ground truth it hasn't earned.
 */
UCLASS(ClassGroup = (WarMachine), meta = (BlueprintSpawnableComponent))
class UWMUtilityAIComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UWMUtilityAIComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtilityAI")
	EWMAIAwarenessLevel AwarenessLevel = EWMAIAwarenessLevel::Unaware;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtilityAI")
	float PerceivedThreatLevel = 0.0f; // 0..1, derived from perception events, not ground truth

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtilityAI")
	float SuppressionLevel = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtilityAI")
	FWMGaugeValue Morale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtilityAI")
	FWMGaugeValue Fatigue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UtilityAI")
	float SquadCohesion = 1.0f;

	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	FName EvaluateBestAction();

	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	void ReportPerceivedContact(float ThreatDelta) { PerceivedThreatLevel = FMath::Clamp(PerceivedThreatLevel + ThreatDelta, 0.0f, 1.0f); RaiseAwareness(); }

	UFUNCTION(BlueprintCallable, Category = "UtilityAI")
	void DecayPerception(float DeltaSeconds, float DecayRate = 0.05f) { PerceivedThreatLevel = FMath::Max(0.0f, PerceivedThreatLevel - DecayRate * DeltaSeconds); }

private:
	void RaiseAwareness();
	TArray<FWMUtilityAction> ScoreActions() const;
};
