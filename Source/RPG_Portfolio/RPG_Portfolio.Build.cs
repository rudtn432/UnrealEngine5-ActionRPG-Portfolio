// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RPG_Portfolio : ModuleRules
{
	public RPG_Portfolio(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "GameplayTasks", "NavigationSystem", "UMG" });
	}
}
