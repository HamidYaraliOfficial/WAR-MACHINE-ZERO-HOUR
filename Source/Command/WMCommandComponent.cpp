#include "WMCommandComponent.h"
#include "WMSquad.h"
#include "WarMachine.h"

UWMCommandComponent::UWMCommandComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWMCommandComponent::RegisterSquad(UWMSquadComponent* Squad)
{
	if (Squad) CommandedSquads.AddUnique(Squad);
}

void UWMCommandComponent::IssueOrderToSquad(FName SquadId, EWMOrderType Order, FVector TargetLocation, FName FireTeamId)
{
	for (UWMSquadComponent* Squad : CommandedSquads)
	{
		if (Squad && Squad->SquadId == SquadId)
		{
			Squad->IssueOrder(Order, TargetLocation, FireTeamId);
			return;
		}
	}
	UE_LOG(LogWarMachine, Warning, TEXT("IssueOrderToSquad: squad %s not found under this command."), *SquadId.ToString());
}

void UWMCommandComponent::ConfigureResource(EWMSupportRequestType Type, int32 Charges, float CooldownSeconds)
{
	FWMCommandResource Res;
	Res.Type = Type;
	Res.ChargesAvailable = Charges;
	Resources.Add(Res);
	ResourceCooldownDurations.Add(Type, CooldownSeconds);
}

bool UWMCommandComponent::RequestSupport(EWMSupportRequestType Type)
{
	for (FWMCommandResource& Res : Resources)
	{
		if (Res.Type == Type)
		{
			if (Res.ChargesAvailable <= 0 || Res.CooldownRemainingSeconds > 0.0f)
			{
				return false;
			}
			Res.ChargesAvailable -= 1;
			Res.CooldownRemainingSeconds = ResourceCooldownDurations.FindRef(Type);
			UE_LOG(LogWarMachine, Log, TEXT("Support requested: type=%d, remaining charges=%d"), (int32)Type, Res.ChargesAvailable);
			return true;
		}
	}
	return false;
}

void UWMCommandComponent::TickCooldowns(float DeltaSeconds)
{
	for (FWMCommandResource& Res : Resources)
	{
		Res.CooldownRemainingSeconds = FMath::Max(0.0f, Res.CooldownRemainingSeconds - DeltaSeconds);
	}
}
