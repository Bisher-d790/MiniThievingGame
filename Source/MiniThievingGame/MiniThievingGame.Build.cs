// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MiniThievingGame : ModuleRules
{
    public MiniThievingGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "UMG", "StateTreeModule", "GameplayStateTreeModule" });
    }
}
