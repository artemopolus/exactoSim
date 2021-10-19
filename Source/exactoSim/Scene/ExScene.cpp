// Fill out your copyright notice in the Description page of Project Settings.


#include "ExScene.h"
#include <string>


// Sets default values
AExScene::AExScene()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AExScene::BeginPlay()
{
	Super::BeginPlay();
	//create object
	std::string path = "Class'/Game/Blueprint/Scene/BP_ExSmplBox.BP_ExSmplBox_C'";
	FString fpath(path.c_str());
	UClass * obj = StaticLoadClass(UObject::StaticClass(), nullptr, *fpath);
	if (obj == nullptr)
		__nop();
	else
	{
		FVector location(0,0,0);
		FRotator rotation(0,0,0);
		FActorSpawnParameters params;
		params.Name = "Test";
		APawn *spawned_obj = static_cast<APawn*>(this->GetWorld()->SpawnActor(obj,&location, &rotation, params));
		__nop();
	}	
}

// Called every frame
void AExScene::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

