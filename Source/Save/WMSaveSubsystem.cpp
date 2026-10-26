#include "WMSaveSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "WarMachine.h"

const FString UWMSaveSubsystem::CrashRecoverySlotName = TEXT("__CrashRecovery");

bool UWMSaveSubsystem::SaveToSlot(UWMSaveGame* SaveData, const FString& SlotName)
{
	if (!SaveData || !SaveData->ValidateIntegrity())
	{
		UE_LOG(LogWarMachine, Error, TEXT("SaveToSlot rejected: failed integrity validation."));
		return false;
	}
	SaveData->SlotName = SlotName;
	SaveData->SavedAtUtc = FDateTime::UtcNow();
	const bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveData, SlotName, 0);
	UE_LOG(LogWarMachine, Log, TEXT("SaveToSlot('%s') -> %s"), *SlotName, bSuccess ? TEXT("OK") : TEXT("FAILED"));
	return bSuccess;
}

UWMSaveGame* UWMSaveSubsystem::LoadFromSlot(const FString& SlotName, bool& bOutSuccess)
{
	bOutSuccess = false;
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, 0)) return nullptr;

	UWMSaveGame* Loaded = Cast<UWMSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (Loaded && Loaded->ValidateIntegrity())
	{
		bOutSuccess = true;
		return Loaded;
	}

	UE_LOG(LogWarMachine, Error, TEXT("LoadFromSlot('%s') failed integrity validation, attempting crash-recovery fallback."), *SlotName);
	if (SlotName != CrashRecoverySlotName && UGameplayStatics::DoesSaveGameExist(CrashRecoverySlotName, 0))
	{
		UWMSaveGame* Recovery = Cast<UWMSaveGame>(UGameplayStatics::LoadGameFromSlot(CrashRecoverySlotName, 0));
		if (Recovery && Recovery->ValidateIntegrity())
		{
			bOutSuccess = true;
			return Recovery;
		}
	}
	return nullptr;
}

void UWMSaveSubsystem::WriteCrashRecoveryCheckpoint(UWMSaveGame* SaveData)
{
	if (SaveData) SaveToSlot(SaveData, CrashRecoverySlotName);
}

TArray<FString> UWMSaveSubsystem::ListAvailableSlots() const
{
	// A production build enumerates the save directory; this keeps a minimal explicit example list
	// plus common slots so the UI has something concrete to bind to out of the box.
	TArray<FString> Slots = { TEXT("CampaignSlot_01"), TEXT("CampaignSlot_02"), TEXT("CampaignSlot_03") };
	Slots.RemoveAll([](const FString& S) { return !UGameplayStatics::DoesSaveGameExist(S, 0); });
	return Slots;
}

bool UWMSaveSubsystem::DeleteSlot(const FString& SlotName)
{
	return UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}
