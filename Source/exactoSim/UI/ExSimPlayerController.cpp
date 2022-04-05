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
	InputComponent->BindAction("mouseLClick", IE_Released, this, &AExSimPlayerController::mouseLRelease);
	InputComponent->BindAction("mouseRClick", IE_Pressed, this, &AExSimPlayerController::mouseRClick);
	InputComponent->BindAction("mouseRClick", IE_Released, this, &AExSimPlayerController::mouseRRelease);
}

void AExSimPlayerController::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);
	if (MouseLDragOn)
	{
		FVector loc, dir;
		if (DeprojectMousePositionToWorld(loc, dir))
		{
			PlayerPtr->moveActor(loc, dir);
		}	
	}
}

void AExSimPlayerController::mouseLClick()
{
	if (!PlayerPtr&&!PlayerPtr->DataStorage)
		return;
	FString output = "Loc: ";
	const FVector2D mouse_pos2d = HUDPtr->getMousePosition();
	output += mouse_pos2d.ToString();
	FVector loc, dir;
	HUDPtr->getMouseProjection(loc, dir);
	output += loc.ToString() + TEXT("\n dir:") + dir.ToString();
	FHitResult hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);

	if (hit.bBlockingHit)
	{
		output += TEXT("\nActor: ") + hit.Actor->GetName() + TEXT(" ");
		

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
				PlayerPtr->editActor(actor, mouse_pos2d, loc, hit.Location);	
			}
		}
	}

	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, output);

}

void AExSimPlayerController::mouseLRelease()
{
	FString output = "Mouse out";
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, output);
	if (MouseLDragOn)
	{
		MouseLDragOn = false;
		PlayerPtr->releaseActor();
	}
}

void AExSimPlayerController::mouseRClick()
{
	if (!PlayerPtr&&!PlayerPtr->DataStorage)
		return;
	FVector loc, dir;
	FHitResult hit;
	const FVector2D mouse_pos2d = HUDPtr->getMousePosition();
	
	HUDPtr->getMouseProjection(loc, dir);
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hit);
	
	if (hit.bBlockingHit)
	{
		if (DeprojectMousePositionToWorld(loc, dir))
		{
			dir = hit.Location - loc;
			AActor* actor = hit.Actor.Get();
		}
	}
}

void AExSimPlayerController::mouseRRelease()
{
}
