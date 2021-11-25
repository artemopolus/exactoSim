// Fill out your copyright notice in the Description page of Project Settings.


#include "ExScene.h"
#include <string>

#include "ExGenerator.h"
#include "exactoSim/Common/ExSimStorage.h"
#include "Generators/CarGenerator.h"


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
	const std::string path = "Class'/Game/Blueprint/Scene/BP_ExGenerator.BP_ExGenerator_C'";
	FString fpath(path.c_str());
	UClass * obj = StaticLoadClass(UObject::StaticClass(), nullptr, *fpath);
	if (obj != nullptr)
	{
		FActorSpawnParameters params;
   		params.Name = "TestGenerator";
   		AExGenerator *spawned_obj = static_cast<AExGenerator*>(this->GetWorld()->SpawnActor(obj,&location, &rotation, params));
		actor_body_storage elem;
		elem.actor = spawned_obj;
		elem.body = nullptr;
		if (ExPhyzX)
		{
			spawned_obj->ParentScene = this;
			//spawned_obj->generateObj();
		}
		SceneObjects.Add(elem);
    
	}
}



void AExScene::sendCmdToSelected(int type, float value)
{
	FVector loc;
	FRotator rot;
	AExGenerator * target = static_cast<AExGenerator*>(SceneObjects[1].actor);

	int value_int = value;
	
	switch (type)
	{
	case 0:
		deleteSceneObjByPrefix(target->getGeneratedObjPrefix());
		//target->generateObj();		
		break;
	case 1:
		loc = target->GetActorLocation();
		loc.Y += value;
		target->SetActorLocation(loc);
		break;
	case 2:
		rot = target->GetActorRotation();
		rot.Roll += value;
		target->SetActorRotation(rot);
		break;
	case 3:
		target->generateObj(FVector(0,0,value));
		break;
	case AExSimStorage::exsim_cmd_type::EXCT_SWITCH:
		UE_LOG(LogTemp, Warning, TEXT("Switch to object number: %d "), value_int  );
		break;
	default:
		break;
	}

}

void AExScene::sendExtendedCmdToSelected(actor_cmd cmd)
{

	AExGenerator * target = static_cast<AExGenerator*> (SceneObjects[1].actor);
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Generate car"));
	FActorSpawnParameters params;
	FVector location(0,0,150);
	FRotator rotation(0,0,90);
	const std::string suffix = "_C'";
	const std::string prefix = "Class'/Game/Blueprint/Scene/";
	const std::string name_basis = "BP_ExSmplBox_";
	std::string name_trg = "Simple";

	std::string path = prefix + name_basis + name_trg + "." + name_basis + name_trg + suffix;
	FString fpath(path.c_str());
	UClass * obj = StaticLoadClass(UObject::StaticClass(), nullptr, *fpath);

	TArray<AExactoPhysics::ConnectedBodies> system;
	AExactoPhysics::ConnectedBodies elem;
	APawn * car_parent = nullptr;
	if (obj != nullptr)
	{
		params.Name = "CarBasic";
		APawn *spawned_obj = static_cast<APawn*>(this->GetWorld()->SpawnActor(obj,&location, &rotation, params));
		car_parent = spawned_obj;
		elem.target = spawned_obj;
		elem.parent = nullptr;
		elem.trg_body = nullptr;
	}
	system.Add(elem);
	name_trg = "Cylinder";
	path = prefix + name_basis + name_trg + "." + name_basis + name_trg + suffix;
	fpath = path.c_str();
	if ((obj = StaticLoadClass(UObject::StaticClass(), nullptr, *fpath)) != nullptr)
	{
		params.Name = "CarWheel0";
		location.Y = -100;
		APawn *spawned_obj = static_cast<APawn*>(this->GetWorld()->SpawnActor(obj,&location, &rotation, params));
		elem.target = spawned_obj;
		elem.parent = car_parent;
		elem.trg_body = nullptr;
							/*btVector3 axisA(0.f, 0.f, 1.f);
        					btVector3 axisB(0.f, 0.f, 1.f);
        					btVector3 pivotA(-0.0f, -0.0f, -0.5f);
        					btVector3 pivotB(0.f, 0.f, 0.f);*/
		elem.axis_p =  FVector(0.f, 0.f, 100.f);
		elem.axis_t =  FVector(0.f, 0.f, 100.f);
		elem.pivot_p = FVector(0.f, 0.f, -50.0f);
		elem.pivot_t = FVector(0.0f, 0.0f, 0.0f);
	}
	system.Add(elem);
	if ((obj = StaticLoadClass(UObject::StaticClass(), nullptr, *fpath)) != nullptr)
	{
		params.Name = "CarWheel1";
		location.Y = 100;
		APawn *spawned_obj = static_cast<APawn*>(this->GetWorld()->SpawnActor(obj,&location, &rotation, params));
		elem.target = spawned_obj;
		elem.parent = car_parent;
		elem.trg_body = nullptr;
		elem.axis_p =  FVector(0.f, 0.f, 100.f);
		elem.axis_t =  FVector(0.f, 0.f, 100.f);
		elem.pivot_p = FVector(0.f, 0.f, 50.0f);
		elem.pivot_t = FVector(0.0f, 0.0f, 0.0f);	}
	system.Add(elem);
	ExPhyzX->AddComplexBody(system);
}

