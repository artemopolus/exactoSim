// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/ExSimFileManager.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "exactoWorld.generated.h"
UCLASS()
class EXACTOSIM_API AExactoWorld : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AExactoWorld();

	AExSimFileManager * ExFileManager;
	UProceduralMeshComponent * Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
