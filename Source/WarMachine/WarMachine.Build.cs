using UnrealBuildTool;

public class WarMachine : ModuleRules
{
	public WarMachine(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnableExceptions = false;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore",
			"AIModule", "NavigationSystem", "GameplayTasks",
			"GameplayAbilities", "GameplayTags", "EnhancedInput",
			"UMG", "Slate", "SlateCore", "CommonUI",
			"ChaosVehicles", "PhysicsCore", "NetCore",
			"Json", "JsonUtilities", "DeveloperSettings"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
	}
}
