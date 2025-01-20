// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AbilityTesterEditor : ModuleRules
{
	public AbilityTesterEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bLegacyPublicIncludePaths = true;
		ShadowVariableWarningLevel = WarningLevel.Error;

		PublicIncludePaths.AddRange(new string[]
		{
			"AbilityTesterEditor/Public",
        });

		PrivateIncludePaths.AddRange(new string[]
			{

				"AbilityTesterEditor/Private",
            });


		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
		PrivateDependencyModuleNames.AddRange(new string[] { 
					"SlateCore",
					"Slate",
					"AssetTools",
					"PropertyEditor",
					"EditorStyle",
					"Kismet",
					"KismetWidgets",
					"ApplicationCore",
					"ToolMenus",
					"UMG",
					"RenderCore",
					"GameplayAbilities",
                    "ScriptableEditorWidgets",
                    "UnrealEd",
					"InputCore",
                    "EditorSubsystem",
                    "DeveloperSettings",
					"ProjectVS"
        });

	}
}
