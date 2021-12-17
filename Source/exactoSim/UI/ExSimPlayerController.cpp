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
	InputComponent->BindAction("mouseLClick", IE_Released, this, &AExSimPlayerController::mouseRelease);
}

void AExSimPlayerController::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);
	if (MouseLDragOn)
	{
		FVector loc, dir;
		if (DeprojectMousePositionToWorld(loc, dir))
		{
			PlayerPtr->moveActor(loc);
		}	
	}
}

void AExSimPlayerController::mouseLClick()
{
	if (!PlayerPtr&&!PlayerPtr->DataStorage)
		return;
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
		/*for(int i = 0; i < hit.Actor->Tags.Num(); i++)
		{
			FString tag = hit.Actor->Tags[i].ToString();
			output += tag + TEXT(";");
			if (tag == PlayerPtr->DataStorage->CurrentScene->BaseTag)
			{
				output += TEXT("[Target]");
				PlayerPtr->setupConstrainOptions(HUDPtr->getMousePosition());
				
			}
		}*/
		

		//FVector loc, dir;
		if (DeprojectMousePositionToWorld(loc, dir))
		{
			output += TEXT("\nhit position:") + hit.Location.ToString();
			
			output += TEXT("\nmouse position: ") + loc.ToString();

			dir = hit.Location - loc;
			output += TEXT("\ndirection: ") + dir.ToString();
			AActor* actor = hit.Actor.Get();
			int mode = PlayerPtr->DataStorage->getMode();
			if (mode == AExSimStorage::es_modes::MOVE)
			{
				PlayerPtr->touchActor(actor, loc, hit.Location);
				MouseLDragOn = true;
			}
			else if (mode == AExSimStorage::es_modes::EDIT)
			{
				
			}
		}
	}

	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, output);
	/*if (PlayerPtr->DataStorage)
		PlayerPtr->DataStorage->clicked();*/

}

void AExSimPlayerController::mouseRelease()
{
	FString output = "Mouse out";
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, output);
	if (MouseLDragOn)
	{
		MouseLDragOn = false;
		PlayerPtr->releaseActor();
	}
}
