// Fill out your copyright notice in the Description page of Project Settings.


#include "ExScene.h"
#include <string>


// Sets default values
AExScene::AExScene()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create object
	std::string path = "Class'exactoSim/Blueprints/Scene/BP_ExSmplBox.BP_ExSmplBox_C'";
	FString fpath(path.c_str());
	UObject * obj = StaticLoadObject(UObject::StaticClass(), nullptr, *fpath);
	if (obj == nullptr)
		__nop();
}

// Called when the game starts or when spawned
void AExScene::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExScene::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

