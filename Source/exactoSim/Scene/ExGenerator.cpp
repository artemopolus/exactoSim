// Fill out your copyright notice in the Description page of Project Settings.


#include "ExGenerator.h"


// Sets default values
AExGenerator::AExGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GeneratedObjPrefix = "gen_obj_";
}

void AExGenerator::generateObj()
{
	//addObjByPath(my_loc, my_rot, path, name);
	generateObj(FVector(0,0,0));

}

void AExGenerator::generateObj(FVector impulse)
{
	if (ParentScene)
	{
		FVector my_loc =  this->GetActorLocation();
		FRotator my_rot = this->GetActorRotation();
		//my	_loc.Y += 100;
		const std::string name = GeneratedObjPrefix + std::to_string(GenObjectCount++);
		static uint8_t i = 0;
		i++;
		std::string path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox.BP_ExSmplBox_C'";
		//if (i%2)
			path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox_Thin.BP_ExSmplBox_Thin_C'";
		//else if (i%3)
		//	path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox_Clmn.BP_ExSmplBox_Clmn_C'";
		path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox_Sphere.BP_ExSmplBox_Sphere_C'";
		std::string prefix = "Class'/Game/Blueprint/Scene/";
		std::string basis = "BP_ExSmplBox_Complex";
		std::string suffix = "_C'";
		path = prefix + basis + "." + basis + suffix;
		ParentScene->addObjByPath(my_loc, my_rot, path, name, impulse);
	}
}

void AExGenerator::setGeneratedObjPrefix(std::string name)
{
	GeneratedObjPrefix = name;
}

std::string AExGenerator::getGeneratedObjPrefix()
{
	return GeneratedObjPrefix;
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



