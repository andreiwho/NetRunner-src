// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NROnlineSessions : ModuleRules
{
    public NROnlineSessions(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { });
        PrivateIncludePaths.AddRange(new string[] { });
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "OnlineSubsystem",
            });

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "OnlineSubsystemSteam",
                "OnlineSubsystemUtils",
            });

        DynamicallyLoadedModuleNames.AddRange(new string[] { });
    }
}
