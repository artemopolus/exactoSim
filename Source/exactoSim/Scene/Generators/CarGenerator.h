// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "exactoSim/Scene/ExGenerator.h"
#include "GameFramework/Actor.h"
#include "CarGenerator.generated.h"

UCLASS()
class EXACTOSIM_API ACarGenerator : public AExGenerator 
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACarGenerator();
	virtual void generateObj() override;
	virtual void generateObj(FVector impulse) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
