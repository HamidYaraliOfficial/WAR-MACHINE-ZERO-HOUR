#include "WMHUD.h"
#include "Blueprint/UserWidget.h"
#include "WarMachine.h"

void AWMHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetOwningPlayerController();
	if (!PC) return;

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = CreateWidget<UUserWidget>(PC, HUDWidgetClass);
		if (HUDWidgetInstance) HUDWidgetInstance->AddToViewport(0);
	}
	else
	{
		UE_LOG(LogWarMachine, Warning, TEXT("AWMHUD: no HUDWidgetClass assigned -- set it on the HUD Blueprint."));
	}
}

void AWMHUD::ShowTacticalMap()
{
	if (!TacticalMapWidgetInstance && TacticalMapWidgetClass && GetOwningPlayerController())
	{
		TacticalMapWidgetInstance = CreateWidget<UUserWidget>(GetOwningPlayerController(), TacticalMapWidgetClass);
	}
	if (TacticalMapWidgetInstance) TacticalMapWidgetInstance->AddToViewport(10);
}

void AWMHUD::HideTacticalMap()
{
	if (TacticalMapWidgetInstance) TacticalMapWidgetInstance->RemoveFromParent();
}

void AWMHUD::ShowCommandWheel()
{
	if (!CommandWheelWidgetInstance && CommandWheelWidgetClass && GetOwningPlayerController())
	{
		CommandWheelWidgetInstance = CreateWidget<UUserWidget>(GetOwningPlayerController(), CommandWheelWidgetClass);
	}
	if (CommandWheelWidgetInstance) CommandWheelWidgetInstance->AddToViewport(20);
}

void AWMHUD::HideCommandWheel()
{
	if (CommandWheelWidgetInstance) CommandWheelWidgetInstance->RemoveFromParent();
}
