// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSimPlayerController.h"


AExSimPlayerController::AExSimPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AExSimPlayerController::BeginPlay()
{
	PlayerPtr = Cast<AExSimPlayer>(GetPawn());
	HUDPtr = Cast<AExSimHUD>(GetHUD());
	Super::BeginPlay();
}

void AExSimPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("mouseLClick", IE_Pressed, this, &AExSimPlayerController::mouseLClick);
}

void AExSimPlayerController::mouseLClick()
{
	FString output = "Loc: ";
	output += HUDPtr->getMousePosition().ToString();
	FVector loc, dir;
	HUDPtr->getMouseProjection(loc, dir);
	output += loc.ToString() + TEXT("\n dir:") + dir.ToString();
	FHitResult hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);

	if (hit.bBlockingHit)
	{
		output += TEXT("\nActor: ") + hit.Actor->GetName() + TEXT(" ");
		for(int i = 0; i < hit.Actor->Tags.Num(); i++)
		{
			FString tag = hit.Actor->Tags[i].ToString();
			output += tag + TEXT(";");
			if (tag == PlayerPtr->DataStorage->CurrentScene->BaseTag)
			{
				output += TEXT("[Target]");
				PlayerPtr->setupConstrainOptions(HUDPtr->getMousePosition());
				
			}
		}
		
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, output);
	if (PlayerPtr->DataStorage)
		PlayerPtr->DataStorage->clicked();

}
