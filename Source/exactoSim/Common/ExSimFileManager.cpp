// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSimFileManager.h"


// Sets default values
AExSimFileManager::AExSimFileManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PathToContentFolder = FPaths::ProjectContentDir();
	UE_LOG(LogTemp, Warning, TEXT("Content Folder : %s  "), *PathToContentFolder  );

	PathToConfig = PathToContentFolder + TEXT("Options/gen_object_config.json");

	UE_LOG(LogTemp, Warning, TEXT("Config file : %s  "), *PathToConfig  );

	PathToFilesFolder = FPlatformProcess::UserDir();
	PathToFilesFolder.Append("exactoSim");
	UE_LOG(LogTemp, Warning, TEXT("Files path : %s  "), *PathToFilesFolder  );

	PathToModelFolder = "J:/Unreal/3dmodels/";

	PathToDataFolder = PathToFilesFolder + TEXT("/Data/");
	PathToComplexFolder = PathToFilesFolder + TEXT("/Complex/");
	
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
	if (!file_manager.DirectoryExists(*PathToComplexFolder))
	{
		if (!file_manager.CreateDirectory(*PathToComplexFolder))
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

// Called when the game starts or when spawned
void AExSimFileManager::BeginPlay()
{
	Super::BeginPlay();




	FString path_to_meshloader = "Class'/Game/Blueprint/Common/BP_ExMeshLoader.BP_ExMeshLoader_C'";
	UClass * object = StaticLoadClass(UObject::StaticClass(), nullptr, *path_to_meshloader);
	if (object != nullptr)
	{
		MeshLoader = static_cast<AExMeshLoader*>(this->GetWorld()->SpawnActor(object));
		//MeshLoader->openModel(getPathToModel("smpl_box.fbx"));
	}
}

// Called every frame
void AExSimFileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FString AExSimFileManager::getPathToModel(FString name)
{
	return PathToModelFolder + name;
}

FString AExSimFileManager::getPathToBlueprint(FString name)
{
	const FString suffix = "_C'";
	const FString folder = "Class'/Game/Blueprint/Scene/";
	const FString name_prefix = "BP_ExSmplBox_";	
	const FString trg_name = name_prefix + name;
	return  folder + trg_name + TEXT(".") + trg_name + suffix;
}

void AExSimFileManager::openModel(FString name)
{
	if (MeshLoader != nullptr)
		MeshLoader->openModel(name);
}

void AExSimFileManager::loadMeshInComponent(UProceduralMeshComponent* target)
{
	if (MeshLoader != nullptr)
		MeshLoader->loadMeshInComponent(target);
}

void AExSimFileManager::saveEsComplexParams(const es_complex_params* src)
{
	FString name = src->string_list.FindRef("Name");
	FString path = this->PathToComplexFolder + name + TEXT(".json");
	FString out;
	TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&out);
	writer->WriteObjectStart();
	for (auto & str : src->string_list)
		writer->WriteValue(str.Key, str.Value);
	for (auto & component : src->components)
	{
		writer->WriteObjectStart(component->string_list.FindRef("Name"));
		for (auto & str : component->string_list)
			writer->WriteValue(str.Key, str.Value);
		for (auto & constraint : component->constraints)
		{
			writer->WriteObjectStart(constraint->string_list.FindRef("Name"));
			for (const auto & s : component->string_list)
				writer->WriteValue(s.Key, s.Value);
			for (const auto & f : constraint->float_list)
				writer->WriteValue(f.Key, f.Value);
			for (const auto & v : constraint->vector_list)
			{
				writer->WriteArrayStart(v.Key);
				writer->WriteValue(v.Value.X);
				writer->WriteValue(v.Value.Y);
				writer->WriteValue(v.Value.Z);
				writer->WriteArrayEnd();
			}
			for (const auto & b : constraint->boolarray_list)
			{
				writer->WriteArrayStart(b.Key);
				for (auto & value : b.Value)
					writer->WriteValue(value);
				writer->WriteArrayEnd();
			}
			writer->WriteObjectEnd();
		}
		writer->WriteObjectEnd();
	}
	writer->WriteObjectEnd();
	writer->Close();
	FFileHelper::SaveStringToFile(out, *path);
}

