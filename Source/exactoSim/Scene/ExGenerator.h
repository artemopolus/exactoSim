// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExSmplBox.h"
#include "exactoSim/ExactoPhysics.h"
#include "ExGenerator.generated.h"

UCLASS()
class EXACTOSIM_API AExGenerator : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<AExSmplBox*> targets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AExactoPhysics * ExPhyzX;
	AExGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
