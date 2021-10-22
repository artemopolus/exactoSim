// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSimPlayer.h"


// Sets default values
AExSimPlayer::AExSimPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AExSimPlayer::activateFunction() 
{
	//some action
	UE_LOG(LogTemp, Warning, TEXT("Activate function!"));
	DataStorage->registerCmdToSelected(0,0.);
}

void AExSimPlayer::activateDifFunction() 
{
	DataStorage->registerCmdToSelected(3,StartImpulse);
}

void AExSimPlayer::moveRight() 
{
	DataStorage->registerCmdToSelected(1, MoveHorizontalStepSz);
}

void AExSimPlayer::moveLeft() 
{
	DataStorage->registerCmdToSelected(1,-MoveHorizontalStepSz);
}

void AExSimPlayer::rotateUp() 
{
	DataStorage->registerCmdToSelected(2,RotateStepSZ);
}

void AExSimPlayer::rotateDown() 
{
	DataStorage->registerCmdToSelected(2,-RotateStepSZ);
}

void AExSimPlayer::sendDataToStorage()
{
	if (DataStorage)
	{
		DataStorage->updateDebugLog(FText::FromString("Hello"));
	}
}

// Called when the game starts or when spawned
void AExSimPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExSimPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AExSimPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("activateFunction", IE_Pressed, this, &AExSimPlayer::activateFunction);
	InputComponent->BindAction("activateDifFunction", IE_Pressed, this, &AExSimPlayer::activateDifFunction);
	InputComponent->BindAction("moveRight",IE_Pressed, this, &AExSimPlayer::moveRight);
	InputComponent->BindAction("moveLeft",IE_Pressed, this, &AExSimPlayer::moveLeft);
	InputComponent->BindAction("rotateUp",IE_Pressed, this, &AExSimPlayer::rotateUp);
	InputComponent->BindAction("rotateDown",IE_Pressed, this, &AExSimPlayer::rotateDown);
	
}

