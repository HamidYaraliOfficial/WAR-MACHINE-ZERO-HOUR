#include "WMGameInstance.h"
#include "WarMachine.h"

void UWMGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogWarMachine, Log, TEXT("WMGameInstance initialized. Save slot: %s"), *ActiveSaveSlotName);
}
