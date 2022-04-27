// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSmplBox.h"


// Sets default values
AExSmplBox::AExSmplBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AExSmplBox::setEScomponent(ExSimComponent* trg)
{
	ESCompPtr = trg;
}

ExSimComponent* AExSmplBox::getEScomponent()
{
	return ESCompPtr;
}

// Called when the game starts or when spawned
void AExSmplBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExSmplBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

