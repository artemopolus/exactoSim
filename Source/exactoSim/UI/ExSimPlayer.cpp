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

void AExSimPlayer::moveRight(float value) 
{
	if (value != 0.f)
		DataStorage->registerCmdToSelected(1, MoveHorizontalStepSz);
}

void AExSimPlayer::moveLeft(float value) 
{
	if (value != 0.f)
		DataStorage->registerCmdToSelected(1,-MoveHorizontalStepSz);
}

void AExSimPlayer::moveAction(FVector loc, FRotator rot)
{
	if (DataStorage->getMode() == AExSimStorage::es_modes::EDIT)
	{
			
	}
	else if (DataStorage->getMode() == AExSimStorage::es_modes::MOVE)
	{
		FRotator rotation = GetActorRotation() + rot;
		FVector direction = rotation.RotateVector(loc);
		SetActorLocation(GetActorLocation() + direction);
		SetActorRotation(rotation);
	}	
}
void AExSimPlayer::moveForward(float value)
{
	if (value != 0.f)
	{
		moveAction(FVector(MoveHorizontalStepSz,0,0), FRotator(0,0,0));
	}
}

void AExSimPlayer::moveBack(float value)
{
	if (value != 0.f)
	{
		moveAction(FVector(-MoveHorizontalStepSz,0,0), FRotator(0,0,0));
	}
}

void AExSimPlayer::moveUp(float value)
{
	if (value != 0.f)
	{
		moveAction(FVector(0,0,MoveVerticalStepSz), FRotator(0,0,0));
	}	
}

void AExSimPlayer::moveDown(float value)
{
	if (value != 0.f)
	{
		moveAction(FVector(0,0,-MoveVerticalStepSz), FRotator(0,0,0));
	}		
}

void AExSimPlayer::rotateUp(float value) 
{
	//DataStorage->registerCmdToSelected(2,RotateStepSZ);
	if (value != 0.f)
		moveAction(FVector(0,0,0), FRotator(0,RotateStepSZ,0));
}

void AExSimPlayer::rotateDown(float value) 
{
	//DataStorage->registerCmdToSelected(2,-RotateStepSZ);
	if (value != 0.f)
		moveAction(FVector(0,0,0), FRotator(0,-RotateStepSZ,0));
}

void AExSimPlayer::rotateRight(float value)
{

	if (value != 0.f)
		moveAction(FVector(0,0,0), FRotator(RotateStepSZ,0,0));

}

void AExSimPlayer::rotateLeft(float value)
{
	if (value != 0.f)
		moveAction(FVector(0,0,0), FRotator(-RotateStepSZ,0,0));
}

void AExSimPlayer::setupConstrainOptions(FVector2D loc)
{
	if(TargetWidget)
	{
		TargetWidget->setupConstrainOptions(loc);
	}
}

void AExSimPlayer::checkSelectedActor(AActor* actor, FVector2D mouse_loc)
{
	for(int i = 0; i < actor->Tags.Num(); i++)
	{
		if (actor->Tags[i].ToString() == DataStorage->CurrentScene->BaseTag)
		{
			
		}
	}
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
	//InputComponent->BindAction("moveRight",IE_Pressed, this, &AExSimPlayer::moveRight);
	//InputComponent->BindAction("moveLeft",IE_Pressed, this, &AExSimPlayer::moveLeft);

	InputComponent->BindAxis("moveRight", this, &AExSimPlayer::moveRight);
	InputComponent->BindAxis("moveLeft", this, &AExSimPlayer::moveLeft);
	InputComponent->BindAxis("moveForward", this, &AExSimPlayer::moveForward);
	InputComponent->BindAxis("moveBack", this, &AExSimPlayer::moveBack);
	InputComponent->BindAxis("moveUp", this, &AExSimPlayer::moveUp);
	InputComponent->BindAxis("moveDown", this, &AExSimPlayer::moveDown);
	
	InputComponent->BindAxis("rotateUp", this, &AExSimPlayer::rotateUp);
	InputComponent->BindAxis("rotateDown", this, &AExSimPlayer::rotateDown);
	InputComponent->BindAxis("rotateLeft", this, &AExSimPlayer::rotateLeft);
	InputComponent->BindAxis("rotateRight", this, &AExSimPlayer::rotateRight);
	
}

