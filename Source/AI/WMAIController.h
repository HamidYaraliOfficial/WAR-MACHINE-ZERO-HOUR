#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "WMTypes.h"
#include "WMAIController.generated.h"

class UWMUtilityAIComponent;
class UBehaviorTree;
class UAIPerceptionComponent;

/**
 * Squad-member AI controller: wires the native UE5 AIPerception system into the Utility AI layer,
 * then runs a Behavior Tree whose subtree selection is driven by the Blackboard key "CurrentUtilityAction".
 * All decisions are based only on perceived stimuli (Sight/Hearing/Damage) forwarded from AIPerception --
 * there is no reading of ground-truth enemy state.
 */
UCLASS()
class AWMAIController : public AAIController
{
	GENERATED_BODY()
public:
	AWMAIController();

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UWMUtilityAIComponent> UtilityAI;

protected:
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, struct FAIStimulus Stimulus);

	virtual void Tick(float DeltaSeconds) override;
};
