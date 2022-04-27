// Fill out your copyright notice in the Description page of Project Settings.


#include "ExMeshLoader.h"




// Sets default values
AExMeshLoader::AExMeshLoader()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AExMeshLoader::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AExMeshLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExMeshLoader::openModel(FString name)
{
	Assimp::Importer importer;
	std::string path(TCHAR_TO_UTF8(*name));
	UE_LOG(LogTemp, Warning, TEXT("Try to load model: %s "), *FString(path.c_str() ) );

	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't load model") );
		return;
	}
	MeshCurrentlyProcessed = 0;
	processNode(scene->mRootNode, scene);
}

void AExMeshLoader::loadMeshInComponent(UProceduralMeshComponent* target)
{
	if (target  == nullptr)
		return;
	for (int32 i = 0; i < MeshCurrentlyProcessed; i++)
	{
		target->CreateMeshSection(i, Vertices[i], Indices[i], Normals[i],
			UVs[i], VertexColors[i], Tangents[i], true);
	}
}

void AExMeshLoader::processMesh(const aiMesh* mesh)
{
	if (Vertices.Num() <= MeshCurrentlyProcessed)
	{
		Vertices.AddZeroed();
		Normals.AddZeroed();
		UVs.AddZeroed();
		Tangents.AddZeroed();
		VertexColors.AddZeroed();
		Indices.AddZeroed();
	}
	uint32 start = MeshCurrentlyProcessed;
	Vertices[start].Empty();
	Normals[start].Empty();
	UVs[start].Empty();
	Tangents[start].Empty();
	VertexColors[start].Empty();
	Indices[start].Empty();
	for (uint32 i = 0; i < mesh->mNumVertices; i++)
	{
		FVector vertex(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		FVector normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		if (mesh->mTextureCoords[0])
		{
			FVector2D uvs(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			UVs[start].Add(uvs);
		}
		else
		{
			UVs[start].Add(FVector2D(0.0f, 0.0f));
		}
		vertex *= 10;
		Vertices[start].Add(vertex);
		Normals[start].Add(normal);
	}

	for (uint32 i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		Indices[start].Add(face.mIndices[2]);
		Indices[start].Add(face.mIndices[1]);
		Indices[start].Add(face.mIndices[0]);
	}
}

void AExMeshLoader::processNode(aiNode* node, const aiScene* scene)
{
	UE_LOG(LogTemp, Warning, TEXT("Processing a node"));
	for (uint32 i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh);
		++MeshCurrentlyProcessed;
	}
	UE_LOG(LogTemp, Warning, TEXT("Meshes loaded: %d"), MeshCurrentlyProcessed);
	
	for (uint32 i = 0; i < node->mNumChildren; i++) {
		UE_LOG(LogTemp, Warning, TEXT("New child in the node"));
		processNode(node->mChildren[i], scene);
	}
	UE_LOG(LogTemp, Warning, TEXT("Processing a node: Done"));
}

