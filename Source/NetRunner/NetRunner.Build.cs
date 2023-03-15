// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NetRunner : ModuleRules
{
	public NetRunner(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay",
			"NROnlineSessions",
		});
	}
}
