#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WMDeveloperConsole.generated.h"

/**
 * Development-build-only console commands for inspecting AI/Mission/Unit/Vehicle/World state.
 * Guarded by UE_BUILD_SHIPPING so none of this compiles into a Shipping package.
 */
UCLASS()
class UWMDeveloperConsole : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
#if !UE_BUILD_SHIPPING
	UFUNCTION(Exec, Category = "WarMachine|Debug")
	void WM_DumpWarState();

	UFUNCTION(Exec, Category = "WarMachine|Debug")
	void WM_DumpActiveObjectives();

	UFUNCTION(Exec, Category = "WarMachine|Debug")
	void WM_SetTimeScale(float Scale);

	UFUNCTION(Exec, Category = "WarMachine|Debug")
	void WM_ToggleAIDebugDraw();
#endif
};
