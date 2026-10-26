#include "WarMachineGameMode.h"
#include "WarMachine.h"
#include "WMWarStateSubsystem.h"
#include "WMMissionDirectorSubsystem.h"
#include "WMCampaignClockSubsystem.h"

AWarMachineGameMode::AWarMachineGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AWarMachineGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	UE_LOG(LogWarMachine, Log, TEXT("WAR MACHINE: ZERO HOUR - Initializing game on map %s"), *MapName);
}

void AWarMachineGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (UWMWarStateSubsystem* WarState = World->GetSubsystem<UWMWarStateSubsystem>())
		{
			WarState->InitializeWarState();
		}
		if (UWMMissionDirectorSubsystem* Director = World->GetSubsystem<UWMMissionDirectorSubsystem>())
		{
			Director->BeginCampaignDay();
		}
		if (UWMCampaignClockSubsystem* Clock = World->GetSubsystem<UWMCampaignClockSubsystem>())
		{
			Clock->StartClock();
		}
	}
}
