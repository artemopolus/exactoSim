// Fill out your copyright notice in the Description page of Project Settings.


#include "ExGenerator.h"


// Sets default values
AExGenerator::AExGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AExGenerator::generateObj()
{
	FVector my_loc =  this->GetActorLocation();
	FRotator my_rot = this->GetActorRotation();
	//my_loc.Y += 100;
	std::string name = "gen_obj";
	const std::string path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox.BP_ExSmplBox_C'";
	addObjByPath(my_loc, my_rot, path, name);
}

// Called when the game starts or when spawned
void AExGenerator::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AExGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExGenerator::addObjByPath(FVector location, FRotator rotation, std::string path, std::string name)
{
	name += "_" + std::to_string(GenObjectCount);
	FString fpath(path.c_str());
	UClass * obj = StaticLoadClass(UObject::StaticClass(), nullptr, *fpath);
	if (obj != nullptr)
	{
			FActorSpawnParameters params;
    		params.Name = name.c_str();
    		APawn *spawned_obj = static_cast<APawn*>(this->GetWorld()->SpawnActor(obj,&location, &rotation, params));
    		if (ExPhyzX)
    		{
    			ExPhyzX->AddRigidBody(spawned_obj);
    		}	
	}
}

