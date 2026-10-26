#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WMTypes.h"
#include "WMObjective.generated.h"

/** A single mission objective node; objectives can depend on each other and can be substituted, not just failed. */
UCLASS(BlueprintType)
class UWMObjective : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly) FGuid ObjectiveId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EWMObjectiveType Type = EWMObjectiveType::Secure;
	UPROPERTY(BlueprintReadWrite) EWMObjectiveState State = EWMObjectiveState::Available;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector Location = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool bIsOptional = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FGuid> DependsOnObjectiveIds;
	/** If this objective becomes unreachable, the Mission Director spawns this fallback type instead of failing the mission. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EWMObjectiveType SubstituteType = EWMObjectiveType::Search;

	UFUNCTION(BlueprintCallable) void MarkCompleted() { State = EWMObjectiveState::Completed; }
	UFUNCTION(BlueprintCallable) void MarkFailed() { State = EWMObjectiveState::Failed; }
	UFUNCTION(BlueprintCallable) void MarkSkipped() { State = EWMObjectiveState::Skipped; }
};
