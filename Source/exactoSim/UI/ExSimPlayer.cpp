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
	DataStorage->registerCmdToSelected(1,1.);
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
}

