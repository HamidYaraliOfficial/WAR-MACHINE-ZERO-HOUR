#pragma once
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "WMTypes.h"
#include "WMObjective.h"
#include "WMMissionDirectorSubsystem.generated.h"

class UWMWarStateSubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectiveStateChanged, UWMObjective*, Objective);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissionCompleted, bool, bSuccess);

/**
 * Generates and evolves mission objectives at runtime from War State, player performance, resources and
 * enemy posture rather than a fixed script. A mission is not a strict line: optional objectives can be
 * skipped (with campaign consequences applied by WMWarStateSubsystem) and an objective that becomes
 * unreachable is substituted rather than failing the whole mission outright.
 */
UCLASS()
class UWMMissionDirectorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Mission")
	TArray<TObjectPtr<UWMObjective>> ActiveObjectives;

	UPROPERTY(BlueprintAssignable, Category = "Mission")
	FOnObjectiveStateChanged OnObjectiveStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Mission")
	FOnMissionCompleted OnMissionCompleted;

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void BeginCampaignDay();

	/** Builds a mission's initial objective graph from the current War/Frontline state. */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	void GenerateMissionObjectives(EWMFaction EnemyFaction, int32 DifficultyBudget);

	UFUNCTION(BlueprintCallable, Category = "Mission")
	UWMObjective* AddObjective(FText Title, EWMObjectiveType Type, FVector Location, bool bOptional);

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void CompleteObjective(UWMObjective* Objective);

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void SkipObjective(UWMObjective* Objective);

	/** Called when the level/geometry/AI state makes an objective impossible; substitutes rather than hard-fails. */
	UFUNCTION(BlueprintCallable, Category = "Mission")
	void SubstituteUnreachableObjective(UWMObjective* Objective);

	UFUNCTION(BlueprintPure, Category = "Mission")
	bool AreAllMandatoryObjectivesResolved() const;

	UFUNCTION(BlueprintCallable, Category = "Mission")
	void EvaluateMissionCompletion();

private:
	bool AreDependenciesMet(const UWMObjective* Objective) const;
};
