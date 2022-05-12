// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSimFileManager.h"

#include "exactoSim/Utils/ExConvert.h"


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

void AExSimFileManager::save(ExSimComplex* target)
{
	es_complex_params * cmplx = new es_complex_params();
	cmplx->string_list.Add("Name",target->getName());
	cmplx->string_list.Add("BasisName",target->getBasis()->getName());

	TMap<EConstraintParamNames, FString> constraint_names_dict;
	TMap<EConstraintParamNames, FString> constraint_values_dict;

	TMap<EnExComponentParamNames, FString> component_names_dict;
	TMap<EnExComponentParamNames, FString> component_values_dict;

	ExConstraintDict::getDefaultNames(&constraint_names_dict);
	ExComponentDict::getDefaultNames(&component_names_dict);
	
	
	for (const auto component : *target->getComponentsList())
	{
		es_component_params * cmpnt = new es_component_params();
		ExComponentDict::updateValues(&component_values_dict, component->getParams());
		ExComponentDict::getNameValuePairs(&component_names_dict, &component_values_dict, &cmpnt->string_list);
		// cmpnt->string_list.Add("Name",component->getName());
		// cmpnt->string_list.Add("Path",component->getPath());
		for (const auto constr : *component->getConstraints())
		{
			es_constraint_params * p = new es_constraint_params();


			ExConstraintDict::updateValues(&constraint_values_dict, constr->getParams());
			ExConstraintDict::getNameValuePairs(&constraint_names_dict,&constraint_values_dict, &p->string_list);

			cmpnt->constraints.Add(p);
		}
		cmplx->components.Add(cmpnt);
	}
	saveEsComplexParams(cmplx);
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
		writer->WriteObjectStart(component->string_list.FindRef("Component Name"));
		for (auto & str : component->string_list)
			writer->WriteValue(str.Key, str.Value);
		for (auto & constraint : component->constraints)
		{
			const auto constraint_name = constraint->string_list.FindRef("Constraint Name");
			writer->WriteObjectStart(constraint_name);
			for (const auto & s : constraint->string_list)
				writer->WriteValue(s.Key, s.Value);
			
			writer->WriteObjectEnd();
		}
		writer->WriteObjectEnd();
	}
	writer->WriteObjectEnd();
	writer->Close();
	FFileHelper::SaveStringToFile(out, *path);
}

void AExSimFileManager::loadEsComplexParams(TArray<es_complex_params *> complexes_list)
{
	IPlatformFile& file_manager = FPlatformFileManager::Get().GetPlatformFile();
	TArray<FString> files_list;
	file_manager.FindFiles(files_list, *PathToComplexFolder, TEXT(".json"));
	for (const auto & filepath : files_list)
	{
		FString str;
		FFileHelper::LoadFileToString(str, *filepath);
		TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(str);
		TSharedPtr<FJsonObject> json_object = MakeShareable(new FJsonObject);
		FJsonSerializer::Deserialize(reader, json_object);
		auto fields = json_object->Values;

		es_complex_params * cmplx = new es_complex_params();
		
		for (const auto & field : fields )
		{
			const TSharedPtr<FJsonObject> * obj;
			FString str0;
			if (field.Value->TryGetObject(obj))
			{
				es_component_params * cmpnt = new es_component_params(); // don't want recursive function
				const TSharedPtr<FJsonObject> * cnstr_obj;
				FString str1;
				auto fs = obj->Get()->Values;
				for (const auto & f: fs)
				{
					if (f.Value->TryGetObject(cnstr_obj))
					{
						es_constraint_params * cnstr = new es_constraint_params();
						float val;
						FString str2;
						FVector vec;
						const TArray<TSharedPtr<FJsonValue>> * arr;
						auto ps = cnstr_obj->Get()->Values;
						for (const auto & p : ps)
						{
							if (p.Value->TryGetNumber(val))
								cnstr->float_list.Add(p.Key, val);
							else if (p.Value->TryGetArray(arr))
							{
								bool res;
								if (arr[0][0].Get()->TryGetBool(res))
								{
									TArray<bool> arr_b;
									for (int i = 0; i < arr[0].Num(); i++)
									{
										if (arr[0][i].Get()->TryGetBool(res))
											arr_b.Add(res);
									}
									cnstr->boolarray_list.Add(p.Key, arr_b);
								}
								else if (arr->Num() == 3)
								{
									float value;
									if( arr[0][0].Get()->TryGetNumber(value))
										vec.X = value;
									if( arr[0][1].Get()->TryGetNumber(value))
										vec.Y = value;									
									if( arr[0][2].Get()->TryGetNumber(value))
										vec.Z = value;
									cnstr->vector_list.Add(p.Key, vec);
								}
							}
							else if (p.Value->TryGetString(str2))
								cnstr->string_list.Add(p.Key, str2);
						}
						cmpnt->constraints.Add(cnstr);
					}
					else if (f.Value->TryGetString(str1))
						cmpnt->string_list.Add(f.Key, str1);
				}
				cmplx->components.Add(cmpnt);
			}
			else if (field.Value->TryGetString(str0))
				cmplx->string_list.Add(field.Key, str0);
		}
		complexes_list.Add(cmplx);
	}
	return;
}

void AExSimFileManager::loadEsComplexParams(const FString name, es_complex_params* trg)
{
	if (!trg)
		return;
	FString path = this->PathToComplexFolder + name + TEXT(".json");
	FString str;
	FFileHelper::LoadFileToString(str, *path);
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(str);
	TSharedPtr<FJsonObject> json_object = MakeShareable(new FJsonObject);
	FJsonSerializer::Deserialize(reader, json_object);
	auto fields = json_object->Values;

	// es_complex_params* cmplx = new es_complex_params();
	for (const auto& field : fields)
	{
		const TSharedPtr<FJsonObject>* obj;
		FString str0;
		if (field.Value->TryGetObject(obj))
		{
			//load component
			es_component_params* cmpnt = new es_component_params(); // don't want recursive function
			const TSharedPtr<FJsonObject>* cnstr_obj;
			FString str1;
			auto fs = obj->Get()->Values;
			for (const auto& f : fs)
			{
				if (f.Value->TryGetObject(cnstr_obj))
				{
					es_constraint_params* cnstr = new es_constraint_params();
					FString str2;
					auto ps = cnstr_obj->Get()->Values;
					for (const auto& p : ps)
					{
						//load constraint value
						if (p.Value->TryGetString(str2))
							cnstr->string_list.Add(p.Key, str2);
					}
					cmpnt->constraints.Add(cnstr);
				}
				else if (f.Value->TryGetString(str1))
					//load component value
					cmpnt->string_list.Add(f.Key, str1);
			}
			trg->components.Add(cmpnt);
		}
		else if (field.Value->TryGetString(str0))
			//load complex params
			trg->string_list.Add(field.Key, str0);
	}
}

void AExSimFileManager::loadEsComplexParams(const FString name, ExSimComplex* trg)
{
	es_complex_params * complex_params = new es_complex_params();
	loadEsComplexParams(name, complex_params);
	auto key = complex_params->string_list.FindRef("Name");
	trg->setName(key);
	key = complex_params->string_list.FindRef("BasisName");
	trg->setBasisName(key);
	for (const auto & component : complex_params->components)
	{
		ExSimComponent * trg_component = new ExSimComponent();
		FExComponentParams * component_params = new FExComponentParams();
		ExComponentDict::fromNameValuePairsToParams(&component->string_list, component_params);
		trg_component->setParams(component_params);
		for (auto & constraint : component->constraints)
		{
			FExConstraintParams * params = new FExConstraintParams();
			ExConstraintDict::fromNameValuePairsToParams(& constraint->string_list, params);
			ExSimConstraintPair * constraint_pair = new ExSimConstraintPair();
			constraint_pair->setParams(params);
			trg_component->addConstraint(constraint_pair);
		}
		trg->addComponent(trg_component);
	}
	deleteExSimComplexParams(complex_params);
}

void AExSimFileManager::deleteExSimComplexParams(es_complex_params* cmplx)
{
	for(int32 i = 0; i < cmplx->components.Num(); i++)
	{
		es_component_params * component = cmplx->components[i];
		for(int32 j = 0; j < component->constraints.Num(); j++)
		{
			es_constraint_params * constraint = component->constraints[j];
			constraint->string_list.Empty();
			delete constraint;
		}
		component->constraints.Empty();
		component->string_list.Empty();
		delete component;
	}
	cmplx->components.Empty();
	delete cmplx;
}

