// Fill out your copyright notice in the Description page of Project Settings.


#include "ExGenerator.h"


// Sets default values
AExGenerator::AExGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GeneratedObjPrefix = "gen_obj_";
	GenObjInfo.enabled = false;
	GenObjInfo.prefix = "Class'/Game/Blueprint/";
	GenObjInfo.folder = "Scene/";
	GenObjInfo.genobjcnt = 0;
	GenObjInfo.genobjname = "gen_obj_";
	GenObjInfo.name = "Simple";
	GenObjInfo.name_prefix = "BP_ExSmplBox_";
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

		//my	_loc.Y += 100;
		/*const std::string name = GeneratedObjPrefix + std::to_string(GenObjectCount++);
		static uint8_t i = 0;
		i++;
		std::string path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox.BP_ExSmplBox_C'";
		//if (i%2)
			path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox_Thin.BP_ExSmplBox_Thin_C'";
		//else if (i%3)
		//	path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox_Clmn.BP_ExSmplBox_Clmn_C'";
		path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox_Sphere.BP_ExSmplBox_Sphere_C'";
		const std::string prefix = "Class'/Game/Blueprint/";
		std::string folder_path = "Scene/";
		std::string basis = "BP_ExSmplBox_Complex";
		const std::string suffix = "_C'";
		basis = "BP_ExSmplBox_Shoes";
		basis = "BP_ExSmplBox_Book";
		basis = "BP_ExSmplBox_Folder";
		path = prefix + folder_path + basis + "." + basis + suffix;*/
		if (GenObjInfo.enabled)
		{
			const std::string name_output = GenObjInfo.genobjname + std::to_string(GenObjInfo.genobjcnt++);
			FVector my_loc =  this->GetActorLocation();
			FRotator my_rot = this->GetActorRotation();
			const std::string path = getPathTo();
			UE_LOG(LogTemp, Warning, TEXT("Path to generated object: %s "), *FString(path.c_str())  );
			
			ParentScene->addObjByPath(my_loc, my_rot, path, name_output, impulse);
			
		}
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



void AExGenerator::setGenObjInfo(AExScene::actor_info info)
{
	GenObjInfo = info;
}

void AExGenerator::setGenObjInfo(std::string name)
{
	GenObjInfo.name = name;
	if (GenObjInfo.enabled == false)
		GenObjInfo.enabled = true;
}

std::string AExGenerator::getPathTo()
{
	const std::string suffix = "_C'";
	const std::string trg_name = GenObjInfo.name_prefix + GenObjInfo.name;
	return  GenObjInfo.prefix + GenObjInfo.folder + trg_name + "." + trg_name + suffix;
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



