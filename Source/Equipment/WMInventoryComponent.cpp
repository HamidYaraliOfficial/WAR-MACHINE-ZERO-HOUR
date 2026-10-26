#include "WMInventoryComponent.h"

UWMInventoryComponent::UWMInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UWMInventoryComponent::EquipItem(EWMLoadoutSlot Slot, FName ItemId, float WeightKg)
{
	UnequipSlot(Slot);
	FWMLoadoutItem Item;
	Item.Slot = Slot;
	Item.ItemId = ItemId;
	Item.WeightKg = WeightKg;
	EquippedItems.Add(Item);
	return true;
}

void UWMInventoryComponent::UnequipSlot(EWMLoadoutSlot Slot)
{
	EquippedItems.RemoveAll([Slot](const FWMLoadoutItem& Item) { return Item.Slot == Slot; });
}

void UWMInventoryComponent::SavePreset(FName PresetName)
{
	FWMLoadoutPreset Preset;
	Preset.PresetName = PresetName;
	Preset.Items = EquippedItems;
	SavedPresets.RemoveAll([PresetName](const FWMLoadoutPreset& P) { return P.PresetName == PresetName; });
	SavedPresets.Add(Preset);
}

bool UWMInventoryComponent::LoadPreset(FName PresetName)
{
	for (const FWMLoadoutPreset& Preset : SavedPresets)
	{
		if (Preset.PresetName == PresetName)
		{
			EquippedItems = Preset.Items;
			return true;
		}
	}
	return false;
}

float UWMInventoryComponent::GetTotalWeightKg() const
{
	float Total = 0.0f;
	for (const FWMLoadoutItem& Item : EquippedItems) Total += Item.WeightKg;
	return Total;
}

float UWMInventoryComponent::GetHandlingPenalty() const
{
	const float Ratio = FMath::Clamp(GetTotalWeightKg() / FMath::Max(1.0f, MaxCarryWeightKg), 0.0f, 1.5f);
	return FMath::Clamp(1.0f - FMath::Max(0.0f, Ratio - 0.7f), 0.4f, 1.0f);
}
