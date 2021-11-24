// Fill out your copyright notice in the Description page of Project Settings.


#include "CarGenerator.h"


// Sets default values
ACarGenerator::ACarGenerator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ACarGenerator::generateObj()
{
}

void ACarGenerator::generateObj(FVector impulse)
{
}

// Called when the game starts or when spawned
void ACarGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACarGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

