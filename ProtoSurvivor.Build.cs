// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ProtoSurvivor : ModuleRules
{
	public ProtoSurvivor(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "CSCoreLibrary" ,"FirebaseFeatures", "NiagaraUIRenderer" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "GameplayTasks" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG", "OnlineSubsystemUtils", "Paper2D" });
			
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
	        PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem" });
	    //   PrivateDependencyModuleNames.Add("OnlineSubsystemGooglePlay");
	        PrivateDependencyModuleNames.Add("AndroidPermission");
        }
        
        if (Target.Platform == UnrealTargetPlatform.Android)
        {
	        string SourcePath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
	        AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(SourcePath, "AndroidAPITemplate_APL.xml"));
        }
	}
}
