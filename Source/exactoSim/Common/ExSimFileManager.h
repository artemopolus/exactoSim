// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExMeshLoader.h"
#include "exactoSim/DataTypes/ExSimComponent.h"
#include "GameFramework/Actor.h"
#include "ExSimFileManager.generated.h"



UCLASS()
class EXACTOSIM_API AExSimFileManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExSimFileManager();


	struct es_constraint_params
	{
		TMap<FString, FVector> vector_list;
		TMap<FString, TArray<bool>> boolarray_list;
		TMap<FString, float> float_list;
		TMap<FString, FString> string_list;
	};

	struct es_component_params
	{
		TArray<es_constraint_params*> constraints;
		TMap<FString, FString> string_list;
	};

	struct es_complex_params
	{
		TArray<es_component_params*> components;
		TMap<FString, FString> string_list;
	};
	
	
private:
	FString PathToConfig;
	FString PathToContentFolder;
	FString PathToFilesFolder;
	FString PathToDataFolder;
	FString PathToComplexFolder;
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

	FString getPathToBlueprint(FString name);

	void openModel(FString name);
	
	void loadMeshInComponent(UProceduralMeshComponent* target);

	void save(ExSimComplex * src);

	void saveEsComplexParams(const es_complex_params * src);

	void loadEsComplexParams(TArray<es_complex_params *> trg);
	void loadEsComplexParams(const FString name, es_complex_params * trg);
	void loadEsComplexParams(const FString name, ExSimComplex * trg);

	void deleteExSimComplexParams(es_complex_params * cmplx);
	
};
