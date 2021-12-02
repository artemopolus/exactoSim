// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSimFileManager.h"


// Sets default values
AExSimFileManager::AExSimFileManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AExSimFileManager::BeginPlay()
{
	Super::BeginPlay();

	PathToContentFolder = FPaths::ProjectContentDir();
	UE_LOG(LogTemp, Warning, TEXT("Content Folder : %s  "), *PathToContentFolder  );

	PathToConfig = PathToContentFolder + TEXT("Options/gen_object_config.json");

	UE_LOG(LogTemp, Warning, TEXT("Config file : %s  "), *PathToConfig  );

	PathToFilesFolder = FPlatformProcess::UserDir();
	PathToFilesFolder.Append("exactoSim");
	UE_LOG(LogTemp, Warning, TEXT("Files path : %s  "), *PathToFilesFolder  );

	PathToDataFolder = PathToFilesFolder + TEXT("Data/");

	DataFileName = TEXT("Session_");

	//создаем файл сессии



	IPlatformFile& file_manager = FPlatformFileManager::Get().GetPlatformFile();
	
	if (!file_manager.DirectoryExists(*PathToFilesFolder))
	{
		if (!file_manager.CreateDirectory(*PathToFilesFolder))
		{
			UE_LOG(LogTemp, Error, TEXT("Can't create basic folder"));
			return;
		}
	}
	if (!file_manager.DirectoryExists(*PathToDataFolder))
	{
		if (!file_manager.CreateDirectory(*PathToDataFolder))
		{
			UE_LOG(LogTemp, Error, TEXT("Can't create basic folder"));
			return;			
		}
	}
	
	FString path_to_session = PathToDataFolder + DataFileName + FDateTime::UtcNow().ToString(TEXT("%Y%m%d_%H%M%S")) + TEXT(".txt");

	UE_LOG(LogTemp, Warning, TEXT("Path to session file : %s  "), *path_to_session  );

	FString init_session_str = TEXT("Start session in ") + FDateTime::UtcNow().ToString();

	FFileHelper::SaveStringToFile(init_session_str,*path_to_session, FFileHelper::EEncodingOptions::ForceUTF8,&IFileManager::Get(),FILEWRITE_None);
}

// Called every frame
void AExSimFileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

