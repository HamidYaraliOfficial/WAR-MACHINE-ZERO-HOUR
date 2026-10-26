#include "WMSquad.h"
#include "WarMachine.h"

UWMSquadComponent::UWMSquadComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SquadId = FName(*FGuid::NewGuid().ToString());
}

UWMSquadMember* UWMSquadComponent::AddMember(const FText& Name, EWMUnitRole Role)
{
	UWMSquadMember* NewMember = NewObject<UWMSquadMember>(this);
	NewMember->MemberId = FGuid::NewGuid();
	NewMember->DisplayName = Name;
	NewMember->Role = Role;
	Members.Add(NewMember);
	return NewMember;
}

FName UWMSquadComponent::CreateFireTeam(const TArray<UWMSquadMember*>& InMembers)
{
	FWMFireTeam Team;
	Team.TeamId = FName(*FString::Printf(TEXT("FT-%d"), FireTeams.Num() + 1));
	for (UWMSquadMember* M : InMembers)
	{
		if (M) Team.Members.Add(M);
	}
	FireTeams.Add(Team);
	return Team.TeamId;
}

void UWMSquadComponent::IssueOrder(EWMOrderType Order, FVector TargetLocation, FName FireTeamId)
{
	// Command efficiency scales with Cohesion: a fractured squad reacts slower / less reliably.
	// Actual movement/behavior execution is delegated to each member's AI Controller (see WMAIController),
	// which reads the last-issued order + Cohesion-derived response delay from this component.
	for (FWMFireTeam& Team : FireTeams)
	{
		if (Team.TeamId == FireTeamId || FireTeamId.IsNone())
		{
			Team.RallyPoint = TargetLocation;
			UE_LOG(LogWarMachine, Log, TEXT("Squad %s: order %d issued to %s at %s (cohesion=%.2f)"),
				*SquadId.ToString(), (int32)Order, *Team.TeamId.ToString(), *TargetLocation.ToString(), Cohesion);
		}
	}
}

void UWMSquadComponent::RecomputeCohesion(float AverageMemberSeparationMeters, float RadioSignalQuality)
{
	const float SeparationFactor = 1.0f - FMath::Clamp(AverageMemberSeparationMeters / MaxEffectiveSeparationMeters, 0.0f, 1.0f);
	const int32 Effective = GetCombatEffectiveCount();
	const float CasualtyFactor = Members.Num() > 0 ? (float)Effective / (float)Members.Num() : 1.0f;

	const float NewCohesion = FMath::Clamp(0.5f * SeparationFactor + 0.3f * CasualtyFactor + 0.2f * RadioSignalQuality, 0.0f, 1.0f);
	const float Delta = NewCohesion - Cohesion;
	Cohesion = NewCohesion;
	OnCohesionChanged.Broadcast(Cohesion, Delta);
}

int32 UWMSquadComponent::GetCombatEffectiveCount() const
{
	int32 Count = 0;
	for (const UWMSquadMember* M : Members)
	{
		if (M && M->IsCombatEffective()) ++Count;
	}
	return Count;
}

void UWMSquadComponent::ReportCasualty(UWMSquadMember* Member)
{
	if (!Member) return;
	OnMemberLost.Broadcast(Member);
	UE_LOG(LogWarMachine, Warning, TEXT("Squad %s lost combat effectiveness of member %s"),
		*SquadId.ToString(), *Member->DisplayName.ToString());
}
