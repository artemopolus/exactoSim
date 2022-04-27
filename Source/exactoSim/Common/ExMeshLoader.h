// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "assimp/Importer.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ProceduralMeshComponent.h"

#include "ExMeshLoader.generated.h"


UCLASS()
class EXACTOSIM_API AExMeshLoader : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExMeshLoader();

private:
	int32 MeshCurrentlyProcessed = 0;
	TArray<TArray<FVector>> Vertices;
	TArray<TArray<int32>> Indices;
	TArray<TArray<FVector>> Normals;
	TArray<TArray<FVector2D>> UVs;
	TArray<TArray<FProcMeshTangent>> Tangents;
	TArray<TArray<FColor>> VertexColors;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void openModel(FString name);

	void loadMeshInComponent(UProceduralMeshComponent * target);

private:
	void processMesh(const aiMesh* mesh);
	void processNode(aiNode* node, const aiScene* scene);
};
