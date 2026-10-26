#include "WMDeveloperConsole.h"
#include "WarMachine.h"
#include "WMWarStateSubsystem.h"
#include "WMMissionDirectorSubsystem.h"
#include "Kismet/GameplayStatics.h"

#if !UE_BUILD_SHIPPING
void UWMDeveloperConsole::WM_DumpWarState()
{
	if (UWorld* World = GetGameInstance() ? GetGameInstance()->GetWorld() : nullptr)
	{
		if (UWMWarStateSubsystem* WarState = World->GetSubsystem<UWMWarStateSubsystem>())
		{
			UE_LOG(LogWarMachine, Display, TEXT("[DEV] WarState Territory=%.2f Supply=%.2f Intel=%.2f Morale=%.2f Infra=%.2f Mil=%.2f"),
				WarState->Indices.TerritoryControl, WarState->Indices.SupplyStrength, WarState->Indices.IntelligenceQuality,
				WarState->Indices.Morale, WarState->Indices.InfrastructureIntegrity, WarState->Indices.MilitaryStrength);
		}
	}
}

void UWMDeveloperConsole::WM_DumpActiveObjectives()
{
	if (UWorld* World = GetGameInstance() ? GetGameInstance()->GetWorld() : nullptr)
	{
		if (UWMMissionDirectorSubsystem* Director = World->GetSubsystem<UWMMissionDirectorSubsystem>())
		{
			for (UWMObjective* Obj : Director->ActiveObjectives)
			{
				UE_LOG(LogWarMachine, Display, TEXT("[DEV] Objective '%s' type=%d state=%d optional=%d"),
					*Obj->Title.ToString(), (int32)Obj->Type, (int32)Obj->State, Obj->bIsOptional);
			}
		}
	}
}

void UWMDeveloperConsole::WM_SetTimeScale(float Scale)
{
	if (UWorld* World = GetGameInstance() ? GetGameInstance()->GetWorld() : nullptr)
	{
		UGameplayStatics::SetGlobalTimeDilation(World, FMath::Clamp(Scale, 0.05f, 10.0f));
	}
}

void UWMDeveloperConsole::WM_ToggleAIDebugDraw()
{
	UE_LOG(LogWarMachine, Display, TEXT("[DEV] AI debug draw toggled (hook up to your AIPerception/BT debug visualization here)."));
}
#endif
