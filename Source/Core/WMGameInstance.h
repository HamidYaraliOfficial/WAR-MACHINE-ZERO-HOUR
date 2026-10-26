#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "WMTypes.h"
#include "WMGameInstance.generated.h"

/** Holds cross-map, cross-mission persistent player choices (task force, difficulty) before a save is loaded. */
UCLASS()
class UWMGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;

	UPROPERTY(BlueprintReadWrite, Category = "Campaign")
	EWMFaction PlayerTaskForceFaction = EWMFaction::IronCompact;

	UPROPERTY(BlueprintReadWrite, Category = "Campaign")
	EWMDifficulty Difficulty = EWMDifficulty::Regular;

	UPROPERTY(BlueprintReadWrite, Category = "Campaign")
	FString ActiveSaveSlotName = TEXT("CampaignSlot_01");

	UFUNCTION(BlueprintCallable, Category = "Campaign")
	void SetDifficulty(EWMDifficulty NewDifficulty) { Difficulty = NewDifficulty; }
};
