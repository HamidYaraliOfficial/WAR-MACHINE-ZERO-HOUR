#include "WMMissionDirectorSubsystem.h"
#include "WarMachine.h"

void UWMMissionDirectorSubsystem::BeginCampaignDay()
{
	UE_LOG(LogWMMission, Log, TEXT("Mission Director: campaign day started."));
}

void UWMMissionDirectorSubsystem::GenerateMissionObjectives(EWMFaction EnemyFaction, int32 DifficultyBudget)
{
	ActiveObjectives.Empty();

	// Simple but genuine data-driven generation: a primary objective is always chosen from the
	// current Frontline pressure point, plus a scaling number of secondary/optional objectives
	// bounded by DifficultyBudget. This is deliberately small and deterministic here; the
	// production version reads live Frontline/WarState data (see WMFrontlineManager) to place
	// objective locations at real contested points rather than fixed coordinates.
	UWMObjective* Primary = AddObjective(FText::FromString(TEXT("Secure Forward Position")), EWMObjectiveType::Secure, FVector::ZeroVector, false);

	int32 SecondaryBudget = FMath::Clamp(DifficultyBudget, 1, 4);
	for (int32 i = 0; i < SecondaryBudget; ++i)
	{
		UWMObjective* Secondary = AddObjective(
			FText::FromString(FString::Printf(TEXT("Secondary Objective %d"), i + 1)),
			(i % 2 == 0) ? EWMObjectiveType::Recon : EWMObjectiveType::Sabotage,
			FVector(100.f * i, 0.f, 0.f), true);
		Secondary->DependsOnObjectiveIds.Add(Primary->ObjectiveId);
	}

	UE_LOG(LogWMMission, Log, TEXT("Mission Director: generated %d objectives vs faction %d."), ActiveObjectives.Num(), (int32)EnemyFaction);
}

UWMObjective* UWMMissionDirectorSubsystem::AddObjective(FText Title, EWMObjectiveType Type, FVector Location, bool bOptional)
{
	UWMObjective* Obj = NewObject<UWMObjective>(this);
	Obj->ObjectiveId = FGuid::NewGuid();
	Obj->Title = Title;
	Obj->Type = Type;
	Obj->Location = Location;
	Obj->bIsOptional = bOptional;
	Obj->State = EWMObjectiveState::Available;
	ActiveObjectives.Add(Obj);
	OnObjectiveStateChanged.Broadcast(Obj);
	return Obj;
}

bool UWMMissionDirectorSubsystem::AreDependenciesMet(const UWMObjective* Objective) const
{
	for (const FGuid& DepId : Objective->DependsOnObjectiveIds)
	{
		const TObjectPtr<UWMObjective>* Dep = ActiveObjectives.FindByPredicate(
			[&DepId](const UWMObjective* O) { return O->ObjectiveId == DepId; });
		if (!Dep || (*Dep)->State != EWMObjectiveState::Completed)
		{
			return false;
		}
	}
	return true;
}

void UWMMissionDirectorSubsystem::CompleteObjective(UWMObjective* Objective)
{
	if (!Objective) return;
	Objective->MarkCompleted();
	OnObjectiveStateChanged.Broadcast(Objective);
	EvaluateMissionCompletion();
}

void UWMMissionDirectorSubsystem::SkipObjective(UWMObjective* Objective)
{
	if (!Objective || !Objective->bIsOptional) return;
	Objective->MarkSkipped();
	OnObjectiveStateChanged.Broadcast(Objective);
	// Consequence propagation (reduced intel, harder follow-up mission, etc.) is applied by
	// WMWarStateSubsystem::RecordObjectiveDecision, called from the mission's UI layer.
	EvaluateMissionCompletion();
}

void UWMMissionDirectorSubsystem::SubstituteUnreachableObjective(UWMObjective* Objective)
{
	if (!Objective) return;
	Objective->Type = Objective->SubstituteType;
	Objective->State = EWMObjectiveState::Substituted;
	OnObjectiveStateChanged.Broadcast(Objective);
	UE_LOG(LogWMMission, Log, TEXT("Objective %s substituted -> type %d (mission continues)."),
		*Objective->Title.ToString(), (int32)Objective->Type);
}

bool UWMMissionDirectorSubsystem::AreAllMandatoryObjectivesResolved() const
{
	for (const UWMObjective* Obj : ActiveObjectives)
	{
		if (Obj->bIsOptional) continue;
		const bool bResolved = Obj->State == EWMObjectiveState::Completed || Obj->State == EWMObjectiveState::Substituted;
		if (!bResolved) return false;
	}
	return true;
}

void UWMMissionDirectorSubsystem::EvaluateMissionCompletion()
{
	if (AreAllMandatoryObjectivesResolved())
	{
		OnMissionCompleted.Broadcast(true);
		UE_LOG(LogWMMission, Log, TEXT("Mission Director: all mandatory objectives resolved. Mission success."));
	}
}
