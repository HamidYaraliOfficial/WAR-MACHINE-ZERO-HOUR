#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WMTypes.h"
#include "WMSquadMember.h"
#include "WMSquad.generated.h"

USTRUCT(BlueprintType)
struct FWMFireTeam
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FName TeamId;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UWMSquadMember>> Members;

	UPROPERTY(BlueprintReadWrite)
	FVector RallyPoint = FVector::ZeroVector;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSquadCohesionChanged, float, NewCohesion, float, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSquadMemberLost, UWMSquadMember*, LostMember);

/**
 * Hierarchical squad command: owns members, splits them into fireteams, tracks formation,
 * rules of engagement, and cohesion (how well the squad shares information / responds to orders).
 */
UCLASS(ClassGroup = (WarMachine), meta = (BlueprintSpawnableComponent))
class UWMSquadComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UWMSquadComponent();

	UPROPERTY(BlueprintReadOnly, Category = "Squad")
	FName SquadId;

	UPROPERTY(BlueprintReadOnly, Category = "Squad")
	TArray<TObjectPtr<UWMSquadMember>> Members;

	UPROPERTY(BlueprintReadOnly, Category = "Squad")
	TArray<FWMFireTeam> FireTeams;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad")
	EWMFormation CurrentFormation = EWMFormation::Wedge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Squad")
	EWMRulesOfEngagement RulesOfEngagement = EWMRulesOfEngagement::ReturnFireOnly;

	/** 0..1: how well the squad shares intel and responds to orders. Degrades with distance/casualties. */
	UPROPERTY(BlueprintReadOnly, Category = "Squad")
	float Cohesion = 1.0f;

	UPROPERTY(BlueprintAssignable, Category = "Squad")
	FOnSquadCohesionChanged OnCohesionChanged;

	UPROPERTY(BlueprintAssignable, Category = "Squad")
	FOnSquadMemberLost OnMemberLost;

	UFUNCTION(BlueprintCallable, Category = "Squad")
	UWMSquadMember* AddMember(const FText& Name, EWMUnitRole Role);

	UFUNCTION(BlueprintCallable, Category = "Squad")
	FName CreateFireTeam(const TArray<UWMSquadMember*>& InMembers);

	UFUNCTION(BlueprintCallable, Category = "Squad")
	void IssueOrder(EWMOrderType Order, FVector TargetLocation, FName FireTeamId);

	UFUNCTION(BlueprintCallable, Category = "Squad")
	void SetFormation(EWMFormation NewFormation) { CurrentFormation = NewFormation; }

	UFUNCTION(BlueprintCallable, Category = "Squad")
	void SetRulesOfEngagement(EWMRulesOfEngagement NewROE) { RulesOfEngagement = NewROE; }

	/** Recomputes Cohesion from member spread, casualties and radio signal quality (0..1 input). */
	UFUNCTION(BlueprintCallable, Category = "Squad")
	void RecomputeCohesion(float AverageMemberSeparationMeters, float RadioSignalQuality);

	UFUNCTION(BlueprintPure, Category = "Squad")
	int32 GetCombatEffectiveCount() const;

	UFUNCTION(BlueprintCallable, Category = "Squad")
	void ReportCasualty(UWMSquadMember* Member);

private:
	static constexpr float MaxEffectiveSeparationMeters = 60.0f;
};
