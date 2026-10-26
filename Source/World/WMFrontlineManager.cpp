#include "WMFrontlineManager.h"
#include "WarMachine.h"

void UWMFrontlineManager::RegisterSector(FName SectorId, FVector Location, EWMFaction InitialOwner)
{
	FWMFrontlineSector Sector;
	Sector.SectorId = SectorId;
	Sector.Location = Location;
	Sector.ControllingFaction = InitialOwner;
	Sectors.Add(Sector);
}

void UWMFrontlineManager::ApplyPressure(FName SectorId, float PressureDelta, EWMFaction Attacker)
{
	for (FWMFrontlineSector& Sector : Sectors)
	{
		if (Sector.SectorId == SectorId)
		{
			Sector.ContestedPressure = FMath::Clamp(Sector.ContestedPressure + PressureDelta, 0.0f, 1.0f);
			if (Sector.ContestedPressure >= 1.0f && Sector.ControllingFaction != Attacker)
			{
				UE_LOG(LogWarMachine, Log, TEXT("Sector %s falls to faction %d."), *SectorId.ToString(), (int32)Attacker);
				Sector.ControllingFaction = Attacker;
				Sector.ContestedPressure = 0.2f; // newly captured sectors start with residual instability
			}
			return;
		}
	}
}

void UWMFrontlineManager::ResolveContestedSectors()
{
	for (FWMFrontlineSector& Sector : Sectors)
	{
		// Sectors slowly stabilize if not actively pressured this cycle (called once per campaign day).
		Sector.ContestedPressure = FMath::Max(0.0f, Sector.ContestedPressure - 0.05f);
	}
}

FWMFrontlineSector UWMFrontlineManager::GetMostContestedSector() const
{
	FWMFrontlineSector Best;
	float BestPressure = -1.0f;
	for (const FWMFrontlineSector& Sector : Sectors)
	{
		if (Sector.ContestedPressure > BestPressure)
		{
			BestPressure = Sector.ContestedPressure;
			Best = Sector;
		}
	}
	return Best;
}
