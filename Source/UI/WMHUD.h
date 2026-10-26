#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "WMHUD.generated.h"

class UUserWidget;

/**
 * Owns creation/teardown of the CommonUI/UMG root widgets (HUD, Tactical Map, Command Wheel,
 * Objective Tracker, Radio Panel, Vehicle HUD). The actual layout/visuals live in UMG Blueprint
 * widget assets bound to these classes; this C++ side only manages lifetime and theme switching.
 */
UCLASS()
class AWMHUD : public AHUD
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<UUserWidget> TacticalMapWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI") TSubclassOf<UUserWidget> CommandWheelWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "UI") void ShowTacticalMap();
	UFUNCTION(BlueprintCallable, Category = "UI") void HideTacticalMap();
	UFUNCTION(BlueprintCallable, Category = "UI") void ShowCommandWheel();
	UFUNCTION(BlueprintCallable, Category = "UI") void HideCommandWheel();

protected:
	virtual void BeginPlay() override;

	UPROPERTY() TObjectPtr<UUserWidget> HUDWidgetInstance;
	UPROPERTY() TObjectPtr<UUserWidget> TacticalMapWidgetInstance;
	UPROPERTY() TObjectPtr<UUserWidget> CommandWheelWidgetInstance;
};
