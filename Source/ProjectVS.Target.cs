// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectVSTarget : TargetRules
{
	public ProjectVSTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		
		ExtraModuleNames.AddRange(new string[] { "ProjectVS", "GameplayAbilities", "PaperZD", "Paper2D"});
		
	}
}
