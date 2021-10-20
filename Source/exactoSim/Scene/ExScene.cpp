// Fill out your copyright notice in the Description page of Project Settings.


#include "ExScene.h"
#include <string>

#include "ExGenerator.h"


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
		if (ExPhyzX)
			spawned_obj->ExPhyzX = ExPhyzX;
   		DynObj.Add(spawned_obj);
    
	}
}

// Called when the game starts or when spawned
void AExScene::BeginPlay()
{
	Super::BeginPlay();
	//create object
	FVector location(0,50,100);
	FRotator rotation(0,0,0);
	addSmplTestObject(location, rotation);
	location.Z = 200;
	addGenerator(location, rotation);
}

// Called every frame
void AExScene::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExScene::addObjByPath(FVector location, FRotator rotation, std::string path, std::string name)
{
	FString fpath(path.c_str());
	UClass * obj = StaticLoadClass(UObject::StaticClass(), nullptr, *fpath);
	if (obj != nullptr)
	{
			FActorSpawnParameters params;
    		params.Name = name.c_str();
    		APawn *spawned_obj = static_cast<APawn*>(this->GetWorld()->SpawnActor(obj,&location, &rotation, params));
    		DynObj.Add(spawned_obj);
    
    		if (ExPhyzX)
    		{
    			ExPhyzX->AddRigidBody(spawned_obj);
    		}	
	}	
}

