#include "WMUtilityAIComponent.h"
#include "WarMachine.h"

UWMUtilityAIComponent::UWMUtilityAIComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWMUtilityAIComponent::RaiseAwareness()
{
	if (PerceivedThreatLevel > 0.75f) AwarenessLevel = EWMAIAwarenessLevel::Engaged;
	else if (PerceivedThreatLevel > 0.5f) AwarenessLevel = EWMAIAwarenessLevel::Alert;
	else if (PerceivedThreatLevel > 0.25f) AwarenessLevel = EWMAIAwarenessLevel::Investigating;
	else if (PerceivedThreatLevel > 0.05f) AwarenessLevel = EWMAIAwarenessLevel::Suspicious;
	else AwarenessLevel = EWMAIAwarenessLevel::Unaware;
}

TArray<FWMUtilityAction> UWMUtilityAIComponent::ScoreActions() const
{
	TArray<FWMUtilityAction> Actions;

	const float FearFactor = FMath::Clamp(SuppressionLevel + (1.0f - Morale.Value) * 0.5f, 0.0f, 1.0f);
	const float StaminaFactor = 1.0f - Fatigue.Value;
	const float CohesionFactor = SquadCohesion;

	Actions.Add({ FName("TakeCover"),   PerceivedThreatLevel * 0.6f + SuppressionLevel * 0.4f });
	Actions.Add({ FName("Suppress"),    PerceivedThreatLevel * 0.5f * StaminaFactor });
	Actions.Add({ FName("Flank"),       PerceivedThreatLevel * 0.4f * StaminaFactor * CohesionFactor });
	Actions.Add({ FName("Retreat"),     FearFactor * (1.0f - CohesionFactor * 0.5f) * (AwarenessLevel == EWMAIAwarenessLevel::Engaged ? 1.0f : 0.3f) });
	Actions.Add({ FName("Regroup"),     (1.0f - CohesionFactor) * 0.6f });
	Actions.Add({ FName("Advance"),     (1.0f - PerceivedThreatLevel) * 0.3f + Morale.Value * 0.2f });
	Actions.Add({ FName("CallForMedic"),FMath::IsNearlyZero(Morale.Value) ? 0.0f : (1.0f - StaminaFactor) * 0.2f });

	return Actions;
}

FName UWMUtilityAIComponent::EvaluateBestAction()
{
	TArray<FWMUtilityAction> Actions = ScoreActions();
	Actions.Sort([](const FWMUtilityAction& A, const FWMUtilityAction& B) { return A.Score > B.Score; });
	const FName Best = Actions.Num() > 0 ? Actions[0].ActionTag : FName("Idle");
	UE_LOG(LogWMAI, Verbose, TEXT("UtilityAI chose '%s' (threat=%.2f, awareness=%d)"), *Best.ToString(), PerceivedThreatLevel, (int32)AwarenessLevel);
	return Best;
}
