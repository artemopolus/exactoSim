// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExMeshLoader.h"
#include "GameFramework/Actor.h"
#include "ExSimFileManager.generated.h"



UCLASS()
class EXACTOSIM_API AExSimFileManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExSimFileManager();


private:
	FString PathToConfig;
	FString PathToContentFolder;
	FString PathToFilesFolder;
	FString PathToDataFolder;
	FString DataFileName;

	FString PathToModelFolder;
	AExMeshLoader * MeshLoader;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString getPathToModel(FString name);

	void openModel(FString name);
	void loadMeshInComponent(UProceduralMeshComponent* target);	
};
