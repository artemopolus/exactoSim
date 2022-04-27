// Fill out your copyright notice in the Description page of Project Settings.


#include "exactoWorld.h"

// Sets default values
AExactoWorld::AExactoWorld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Test111111111"));

}

// Called when the game starts or when spawned
void AExactoWorld::BeginPlay()
{
	Super::BeginPlay();

	// Blueprint'/Game/Blueprint/Common/BP_ExSimFileManager.BP_ExSimFileManager'
	//J:/WorkspaceFast/exactoSim/Content/Blueprint/Common/BP_ExSimFileManager.uasset

	FString filemanager_path = "Class'/Game/Blueprint/Common/BP_ExSimFileManager.BP_ExSimFileManager_C'";
	UClass * object = StaticLoadClass(UObject::StaticClass(), nullptr, *filemanager_path);

	if (object != nullptr)
	{
		ExFileManager = static_cast<AExSimFileManager*>(this->GetWorld()->SpawnActor(object));
	}

	ExFileManager->openModel(ExFileManager->getPathToModel("smpl_box.fbx"));


	if (Mesh)
	{
		Mesh->CastShadow = true;
		Mesh->SetCollisionObjectType(ECC_WorldStatic);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		Mesh->UpdateCollisionProfile();
		RootComponent = Mesh;

		ExFileManager->loadMeshInComponent(Mesh);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't create mesh"));
	}
}

// Called every frame
void AExactoWorld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

