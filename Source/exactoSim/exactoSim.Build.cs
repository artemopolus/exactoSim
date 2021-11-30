// Copyright Epic Games, Inc. All Rights Reserved.
using System;
using System.IO;
using UnrealBuildTool;

public class exactoSim : ModuleRules
{
	public exactoSim(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Json", "JsonUtilities", "Networking","Sockets" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
		AddBullet();
	}
	private string ThirdPartyPath
	{
		get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../../../ThirdParty/")); }
	}
	protected void AddBullet()
	{
		// This is real basic, only for a single platform & config (Win64)
		// If you build for more variants you'll have to do some more work here

		bool bDebug = Target.Configuration == UnrealTargetConfiguration.Debug || Target.Configuration == UnrealTargetConfiguration.DebugGame;
		bool bDevelopment = Target.Configuration == UnrealTargetConfiguration.Development;

		string buildFolder = bDebug ? "Debug" :
			bDevelopment ? "RelWithDebInfo" : "Release";
		string buildSuffix = bDebug ? "_Debug" :
			bDevelopment ? "_RelWithDebugInfo" : "";

		// Library path
		string librariesPath = Path.Combine(ThirdPartyPath, "lib", "bullet", buildFolder);
		PublicAdditionalLibraries.Add(Path.Combine(librariesPath, "BulletCollision" + buildSuffix + ".lib"));
		PublicAdditionalLibraries.Add(Path.Combine(librariesPath, "BulletDynamics" + buildSuffix + ".lib"));
		PublicAdditionalLibraries.Add(Path.Combine(librariesPath, "LinearMath" + buildSuffix + ".lib"));

		// Include path (I'm just using the source here since Bullet has mixed src & headers)
		PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "bullet3", "src"));
		PublicDefinitions.Add("WITH_BULLET_BINDING=1");

	}
}
