#include "WMSquadMember.h"
#include "WarMachine.h"

void UWMSquadMember::ApplyDamage(float Amount, EWMDamageType DamageType)
{
	if (UnitState == EWMUnitState::KIA) return;

	// Discipline dampens the effective damage of non-lethal suppression-adjacent hits; pure Kinetic/Explosive are unaffected.
	Health.Add(-Amount);
	Confidence.Add(-Amount * 0.4f);

	if (Health.Value <= 0.0f)
	{
		UnitState = (DamageType == EWMDamageType::Explosive) ? EWMUnitState::KIA : EWMUnitState::Downed;
	}
	else if (Health.Value < 0.5f)
	{
		UnitState = EWMUnitState::Wounded;
	}

	UE_LOG(LogWarMachine, Verbose, TEXT("%s took %.1f damage, health now %.2f, state=%d"),
		*DisplayName.ToString(), Amount, Health.Value, (int32)UnitState);
}

void UWMSquadMember::ApplySuppression(float Intensity)
{
	Confidence.Add(-Intensity);
	Fatigue.Add(Intensity * 0.5f);
	if (Confidence.Value < 0.2f && Discipline.Value < 0.4f)
	{
		UnitState = EWMUnitState::Suppressed;
	}
}

void UWMSquadMember::GrantExperience(int32 Amount)
{
	Experience += Amount;
	// Every 100 XP is a coarse Trait-unlock threshold; the actual Trait pool is data-driven and
	// selected by the progression subsystem based on Role and mission performance.
	if (Experience % 100 < Amount)
	{
		UE_LOG(LogWarMachine, Log, TEXT("%s reached an experience milestone (%d XP)."), *DisplayName.ToString(), Experience);
	}
}
