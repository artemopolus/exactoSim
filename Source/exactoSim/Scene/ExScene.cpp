// Fill out your copyright notice in the Description page of Project Settings.


#include "ExScene.h"
#include <string>
//#include "ExGenerator.h"

#include "exactoSim/Common/ExSimStorage.h"
#include "Generators/CarGenerator.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"

// Sets default values
AExScene::AExScene()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

void AExScene::addSmplTestObject(FVector location, FRotator rotation)
{
	const std::string path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox.BP_ExSmplBox_C'";
	addObjByPath(location, rotation, path, "test");
}

void AExScene::addGenerator(FVector location, FRotator rotation)
{
	if (CurrentGenerator == nullptr)
	{
		const std::string path = "Class'/Game/Blueprint/Scene/BP_ExGenerator.BP_ExGenerator_C'";
		FString fpath(path.c_str());
		UClass * obj = StaticLoadClass(UObject::StaticClass(), nullptr, *fpath);
		if (obj != nullptr)
		{
			FActorSpawnParameters params;
			params.Name = "TestGenerator";
			AExGenerator *spawned_obj = static_cast<AExGenerator*>(this->GetWorld()->SpawnActor(obj,&location, &rotation, params));
			spawned_obj->Tags.Add(ToCStr(GeneratorTag));
			actor_body_storage elem;
			elem.actor = spawned_obj;
			elem.body = nullptr;
			if (ExPhyzX)
			{
				spawned_obj->ParentScene = this;
				//spawned_obj->generateObj();
			}
			SceneObjects.Add(elem);
			CurrentGenerator = spawned_obj;
    
		}
	}
}



void AExScene::sendCmdToSelected(int type, float value)
{
	FVector loc;
	FRotator rot;
	AExGenerator * target = nullptr;
	if (SceneObjects.Num() > 0)
		target = static_cast<AExGenerator*>(SceneObjects[1].actor);

	int value_int = value;
	
	switch (type)
	{
	case 0:
		// deleteSceneObjByPrefix(target->getGeneratedObjPrefix());
		//target->generateObj();
		removeConstrain();
		break;
	case 1:
		if (target != nullptr)
		loc = target->GetActorLocation();
		loc.Y += value;
		if (target != nullptr)
		target->SetActorLocation(loc);
		break;
	case 2:
		if (target != nullptr)
		rot = target->GetActorRotation();
		rot.Roll += value;
		if (target != nullptr)
		target->SetActorRotation(rot);
		break;
	case 3:
		if (target != nullptr)
		target->generateObj(FVector(0,0,value));
		removeCar();
		break;
	case AExSimStorage::exsim_cmd_type::EXCT_SWITCH:
		UE_LOG(LogTemp, Warning, TEXT("Switch to object number: %d "), value_int  );
		break;
	default:
		break;
	}

}

void AExScene::moveGenerator(FVector loc, FRotator rot)
{
	FString output = "Generator: ";
	if (CurrentGenerator)
	{
		output += "move";
		CurrentGenerator->SetActorLocation(CurrentGenerator->GetActorLocation() + loc);
		CurrentGenerator->SetActorRotation(CurrentGenerator->GetActorRotation() + rot);
	}
	else
	{
		output += "create";
		addGenerator(SpawnGeneratorLoc, FRotator(0,0,0));
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, output);
}

void AExScene::sendExtendedCmdToSelected(actor_cmd cmd)
{
	if (CurrentGenerator != nullptr)
	{
		AExGenerator * target = CurrentGenerator;
		switch (cmd.value_int)
		{
		case AExSimStorage::exsim_cmd_type::EXCT_SWITCH:
			UE_LOG(LogTemp, Warning, TEXT("Switch to object number: %s "), *FString(cmd.value_str.c_str() ) );
			target->setGenObjInfo(cmd.value_str);
			break;
		default:
			break;		
		}
	}
}

// Called when the game starts or when spawned
void AExScene::BeginPlay()
{
	Super::BeginPlay();
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Screen Message"));
	//create object
	FRotator rotation(0,0,0);
	//addSmplTestObject(SpawnObjectLoc, rotation);
	//addGenerator(SpawnGeneratorLoc, rotation);
	//addCarGen(SpawnGeneratorLoc, rotation);
	generateCar();
}

// Called every frame
void AExScene::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExScene::addObjByPath(FVector location, FRotator rotation, std::string path, std::string name)
{
	addObjByPath(location, rotation, path, name, FVector(0,0,0));	
}

void AExScene::addObjByPath(FVector location, FRotator rotation, std::string path, std::string name, FVector impulse)
{
	FString fpath(path.c_str());
	UClass * obj = StaticLoadClass(UObject::StaticClass(), nullptr, *fpath);
	if (obj != nullptr)
	{
		FActorSpawnParameters params;
		params.Name = name.c_str();
		APawn *spawned_obj = static_cast<APawn*>(this->GetWorld()->SpawnActor(obj,&location, &rotation, params));
		actor_body_storage elem;
		elem.actor = spawned_obj;
		elem.body = nullptr;
		if (ExPhyzX)
		{
			elem.body = ExPhyzX->AddRigidBody(spawned_obj);
			elem.body->applyCentralImpulse( BulletHelpers::ToBtSize(rotation.RotateVector(impulse)));
		}
		SceneObjects.Add(elem);
	}	
}

bool AExScene::addObjByPath(const FString path, const FString name, btRigidBody** body, FVector location,
	FRotator rotation, bool use_genloc, FVector impulse, FVector impulse_pos)
{
	if (use_genloc && CurrentGenerator)
	{
		location = CurrentGenerator->GetActorLocation();
		rotation = CurrentGenerator->GetActorRotation();
	}
	bool result = false;
	UClass * obj = StaticLoadClass(UObject::StaticClass(), nullptr, *path);
	if ((obj != nullptr)&&ExPhyzX)
	{
		FActorSpawnParameters params;
		params.Name = FName(name);
		APawn *spawned_obj = static_cast<APawn*>(this->GetWorld()->SpawnActor(obj,&location, &rotation, params));
		spawned_obj->Tags.Add(ToCStr(BaseTag));
		spawned_obj->Tags.Add(ToCStr(PhysicsTag));
		spawned_obj->Tags.Add(ToCStr(DynamicTag));
		*body = ExPhyzX->AddRigidBody(spawned_obj);
		result = true;
	}
	return result;
}


void AExScene::addCarGen(FVector location, FRotator rotation)
{

}
void AExScene::deleteSceneObjByPrefix(std::string prefix)
{
	for (int32 i = 0; i < SceneObjects.Num(); i++)
	{
		auto elem = SceneObjects[i];
		std::string name = TCHAR_TO_UTF8(*elem.actor->GetName());
		if (name.rfind(prefix,0) == 0)
		{
			ExPhyzX->removeRigidBody(elem.body);
			elem.actor->Destroy();
			SceneObjects.RemoveAt(i);
			break;
		}		
	}
}

void AExScene::generateCar()
{

	FString path_to_config = FPaths::ProjectContentDir() + TEXT("Options/gen_object_config.json");

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, path_to_config);

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*path_to_config))
	{
		return;
	}

	FString json;
	FFileHelper::LoadFileToString(json, *path_to_config);
	/*TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonStr);
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	FJsonSerializer::Deserialize(JsonReader, JsonObject);

	TSharedPtr<FJsonObject> player = JsonObject->GetObjectField("player");*/
	TSharedRef<TJsonReader<TCHAR>> json_reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> json_object = MakeShareable(new FJsonObject);
	FJsonSerializer::Deserialize(json_reader, json_object);
	TSharedPtr<FJsonObject> obj_list = json_object->GetObjectField("Objects");
	TSharedPtr<FJsonObject> default_path_to_bp = json_object->GetObjectField("DefaultPathToBP");
	FString dflt_path_prefix = default_path_to_bp->GetStringField("Prefix");	
	FString dflt_path_folder = default_path_to_bp->GetStringField("Folder");
	FString dflt_path_suffix = default_path_to_bp->GetStringField("Suffix");
	FString dflt_path_basis = default_path_to_bp->GetStringField("Basis");
	FString dflt_path_name = default_path_to_bp->GetStringField("Name");


	//Инициализация
	std::string name_trg = "Simple";
	int complex_obj_num;
	complex_obj_num = obj_list->Values.Num();
	auto complex_list = obj_list->Values;
	UE_LOG(LogTemp, Warning, TEXT("Complex objects count : %d "), complex_obj_num  );
	//перечисляем объекты
	for (auto object_in_complex : complex_list)
	{
		FString obj_name = object_in_complex.Key;
		TSharedPtr<FJsonObject> one_obj = obj_list->GetObjectField(obj_name);
		auto elem_list = one_obj->Values;
		int complex_elem_num = elem_list.Num();
		UE_LOG(LogTemp, Warning, TEXT("elements : %s == %d "), *obj_name, complex_elem_num  );
		//перечисляем элементы
		TArray<AExactoPhysics::ConnectedBodies> system;
		for(auto elem_in_object : elem_list)
		{
			FString elem_name = elem_in_object.Key;
			TSharedPtr<FJsonObject> one_elem = one_obj->GetObjectField(elem_name);
			auto param_list = one_elem->Values;
			int param_elem_num = param_list.Num();
			UE_LOG(LogTemp, Warning, TEXT("params : %s == %d "), *elem_name, param_elem_num  );
			//перечисляем параметры
			TSharedPtr<FJsonObject> path_params = one_elem->GetObjectField("PathToBP");
			FString trg_folder = path_params->GetStringField("Folder");
			FString trg_basis = path_params->GetStringField("Basis");
			FString trg_name = path_params->GetStringField("Name");

			FString res_name = trg_basis + trg_name;
			FString res_path = dflt_path_prefix + trg_folder + res_name + TEXT(".") + res_name + dflt_path_suffix;
			UE_LOG(LogTemp, Warning, TEXT("path : %s  "), *res_path  );

			UClass * class_obj = StaticLoadClass(UObject::StaticClass(), nullptr, *res_path);
			
			if (class_obj != nullptr)
			{
				AExactoPhysics::ConnectedBodies elem;
				FActorSpawnParameters spawn_params;
				FVector location;
        			
				TArray<TSharedPtr<FJsonValue>> location_arr = one_elem->GetArrayField("Location");
				location.X = location_arr[0]->AsNumber();
				location.Y = location_arr[1]->AsNumber();
				location.Z = location_arr[2]->AsNumber();
				UE_LOG(LogTemp, Warning, TEXT("location : %s  "), *location.ToString()  );
        
				TSharedPtr<FJsonObject> parent_params = one_elem->GetObjectField("Parent");
				if (parent_params->GetBoolField("enabled"))
				{
					TArray<TSharedPtr<FJsonValue>> arr;
					elem.name_p = parent_params->GetStringField("Name");
					arr = parent_params->GetArrayField("AxisTarget");
					elem.axis_t = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());
					arr = parent_params->GetArrayField("AxisParent");
					elem.axis_p = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());
					arr = parent_params->GetArrayField("PivotTarget");
					elem.pivot_t = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());
					arr = parent_params->GetArrayField("PivotParent");
					elem.pivot_p = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());
					FString constr_type = parent_params->GetStringField("Constr");
					if (constr_type == "Hinge")
						elem.constr_type = BulletHelpers::HINGE;
					else if (constr_type == "Hinge2")
						elem.constr_type = BulletHelpers::HINGE2;
					else if (constr_type == "G6DOF_Spring")
						elem.constr_type = BulletHelpers::GEN6DOF_SPRING;
					else
						elem.constr_type = BulletHelpers::HINGE;
					arr = parent_params->GetArrayField("EnSpring");
					for (int i = 0; i < 6; i++)
						elem.en_spring[i] = arr[i]->AsNumber();
					arr = parent_params->GetArrayField("UppLimLin");
					elem.upp_lim_lin = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					arr = parent_params->GetArrayField("LowLimLin");
					elem.low_lim_lin = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					arr = parent_params->GetArrayField("UppLimAng");
					elem.upp_lim_ang = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					arr = parent_params->GetArrayField("LowLimAng");
					elem.low_lim_ang = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					arr = parent_params->GetArrayField("StiffLin");
					elem.stiff_lin = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					arr = parent_params->GetArrayField("StiffAng");
					elem.stiff_ang = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					arr = parent_params->GetArrayField("DumpLin");
					elem.dump_lin = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					arr = parent_params->GetArrayField("DumpAng");
					elem.dump_ang = FVector(arr[0]->AsNumber(),arr[1]->AsNumber(),arr[2]->AsNumber());	
					
				}
				else
				{
					elem.name_p = "";
				}
				TArray<TSharedPtr<FJsonValue>> rotation_arr = one_elem->GetArrayField("Rotation");
				FRotator rotation(rotation_arr[0]->AsNumber(), rotation_arr[1]->AsNumber(), rotation_arr[2]->AsNumber());	
				UE_LOG(LogTemp, Warning, TEXT("rotation : %s  "), *rotation.ToString()  );
				elem.name_t = one_elem->GetStringField("Name");
				spawn_params.Name = FName(obj_name + TEXT("_") + elem_name + TEXT("_") + FString::FromInt(SystemsList.Num()));
				
				APawn *spawned_obj = static_cast<APawn*>(this->GetWorld()->SpawnActor(class_obj,&location, &rotation, spawn_params));
				spawned_obj->Tags.Add(ToCStr(BaseTag));
				spawned_obj->Tags.Add(ToCStr(PhysicsTag));
				spawned_obj->Tags.Add(ToCStr(DynamicTag));
				
				elem.target = spawned_obj;
				elem.parent = nullptr;
				elem.trg_body = nullptr;
				elem.trg_constr = nullptr;
				system.Add(elem);
			}
		}
		if ( system.Num() > 0 )
		{
			if (system.Num() > 1)
			{
				for (auto elem : system)
				{
					FString parent_name = elem.name_t;
					AActor * parent_actor = elem.target;
					for (auto & another_elem : system)
					{
						if (parent_name == another_elem.name_p)
						{
							another_elem.parent = parent_actor;
						}
					}
				}
			}
			ExPhyzX->AddComplexBody(&system);
			SystemsList.Add(system);
		}
	}

}

void AExScene::removeCar()
{
	for (int i = 0; i < SystemsList.Num(); i++)
	{
		auto & system = SystemsList[i];
		UE_LOG(LogTemp, Warning, TEXT("System components :  %d "),  system.Num()  );
		for(int j = 0; j < system.Num(); j++)
		{
			auto & component = system[j];
			ExPhyzX->removeRigidBody(component.trg_body);
			component.target->Destroy();
			component.parent = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("Remove : %s == %d "), *component.name_t, j  );
		}
		system.Empty();
	}
	SystemsList.Empty();
}

void AExScene::removeConstrain()
{
	for (auto & system : SystemsList)
	{
		for (auto & component : system)
		{
			if (component.trg_constr != nullptr)
			{
				ExPhyzX->removeConstrain(component.trg_constr);
				component.trg_constr = nullptr;
			}
		}
	}
}

