#include "WMAIController.h"
#include "WMUtilityAIComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Damage.h"
#include "WarMachine.h"

AWMAIController::AWMAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	UtilityAI = CreateDefaultSubobject<UWMUtilityAIComponent>(TEXT("UtilityAI"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception")));
}

void AWMAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (GetPerceptionComponent())
	{
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AWMAIController::OnTargetPerceptionUpdated);
	}

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
	else
	{
		UE_LOG(LogWMAI, Warning, TEXT("AWMAIController possessed %s with no BehaviorTreeAsset assigned."), *GetNameSafe(InPawn));
	}
}

void AWMAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!UtilityAI) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		// Sight/Hearing/Damage stimuli each contribute differently to perceived threat -- this is the
		// only channel through which the AI learns about the world; no shortcuts to ground truth.
		float ThreatDelta = 0.1f;
		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Damage>()) ThreatDelta = 0.35f;
		else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>()) ThreatDelta = 0.2f;
		else if (Stimulus.Type == UAISense::GetSenseID<UAISense_Hearing>()) ThreatDelta = 0.08f;

		UtilityAI->ReportPerceivedContact(ThreatDelta);

		if (UBlackboardComponent* BB = GetBlackboardComponent())
		{
			BB->SetValueAsVector(FName("LastKnownEnemyLocation"), Actor ? Actor->GetActorLocation() : GetPawn()->GetActorLocation());
		}
	}
}

void AWMAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!UtilityAI) return;

	UtilityAI->DecayPerception(DeltaSeconds);
	const FName BestAction = UtilityAI->EvaluateBestAction();

	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		BB->SetValueAsName(FName("CurrentUtilityAction"), BestAction);
		BB->SetValueAsEnum(FName("AwarenessLevel"), (uint8)UtilityAI->AwarenessLevel);
	}
}
