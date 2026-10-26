#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WMDialogueSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FWMDialogueChoice
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText ChoiceText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName NextNodeId;
	/** Gameplay tag that must be present in the Evidence/Decision set for this choice to be shown. Empty = always available. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName RequiredConditionTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float TrustDelta = 0.0f;
};

USTRUCT(BlueprintType)
struct FWMDialogueNode
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName NodeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName SpeakerCharacterId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Line;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FWMDialogueChoice> Choices;
};

/**
 * Branching dialogue graph gated by conditions (previous decisions, mission state, discovered evidence)
 * and by a persistent Trust value per character, rather than a flat linear script.
 */
UCLASS()
class UWMDialogueSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue") TMap<FName, float> CharacterTrust;
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue") TSet<FName> KnownConditionTags; // decisions/evidence collected so far

	UFUNCTION(BlueprintCallable, Category = "Dialogue") void RegisterGraph(const TArray<FWMDialogueNode>& Nodes);
	UFUNCTION(BlueprintCallable, Category = "Dialogue") void AddConditionTag(FName Tag) { KnownConditionTags.Add(Tag); }
	UFUNCTION(BlueprintCallable, Category = "Dialogue") void AdjustTrust(FName CharacterId, float Delta);
	UFUNCTION(BlueprintPure, Category = "Dialogue") float GetTrust(FName CharacterId) const { return CharacterTrust.FindRef(CharacterId); }
	UFUNCTION(BlueprintCallable, Category = "Dialogue") TArray<FWMDialogueChoice> GetAvailableChoices(FName NodeId) const;
	UFUNCTION(BlueprintCallable, Category = "Dialogue") bool GetNode(FName NodeId, FWMDialogueNode& OutNode) const;

private:
	UPROPERTY() TMap<FName, FWMDialogueNode> Graph;
};
