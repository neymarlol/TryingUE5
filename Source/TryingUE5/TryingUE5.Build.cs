// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TryingUE5 : ModuleRules
{
	public TryingUE5(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
