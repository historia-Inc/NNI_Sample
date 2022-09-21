// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class NNI : ModuleRules
{
	public NNI(ReadOnlyTargetRules Target) : base(Target)
	{
        PrivateIncludePaths.AddRange(
		   new string[] {
						//"../../../../../Source/Runtime/Renderer/Private",
						Path.Combine(EngineDirectory, "Source/Runtime/Renderer/Private"),
			   // ... add other private include paths required here ...
		   }
		);

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NeuralNetworkInference"});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
