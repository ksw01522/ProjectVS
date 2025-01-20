// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectVS : ModuleRules
{
    public ProjectVS(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        bLegacyPublicIncludePaths = true;

        PublicIncludePaths.AddRange(new string[]
        {
            "ProjectVS/Public"
        });

        PrivateIncludePaths.AddRange(new string[]
        {
            "ProjectVS/Private"
        });


        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara",
                                            "EnhancedInput", "GameplayAbilities", "GameplayTasks", "GameplayTags", "Slate", "PaperZD", "Paper2D"});


        if(Target.Type == TargetType.Editor)
        {
            PublicDependencyModuleNames.Add("AbilityTesterEditor");
        }

    }
}
