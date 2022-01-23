// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSimPlayer.h"


// Sets default values
AExSimPlayer::AExSimPlayer()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<UUserWidget> MenuClassFinder(TEXT("WidgetBlueprint'/Game/Blueprint/UI/BP_ExEditable'"));
	OptionClass = MenuClassFinder.Succeeded()? MenuClassFinder.Class : nullptr;
	ConstructorHelpers::FClassFinder<UUserWidget> ButtonClassFinder(TEXT("WidgetBlueprint'/Game/Blueprint/UI/BP_ExButton'"));
	ButtonClass = ButtonClassFinder.Succeeded() ? ButtonClassFinder.Class : nullptr;
	ConstructorHelpers::FClassFinder<UUserWidget> SelectorClassFinder(TEXT("WidgetBlueprint'/Game/Blueprint/UI/BP_ExSelector'"));
	//WidgetBlueprint'/Game/Blueprint/UI/BP_ExSelector.BP_ExSelector'
	SelectorClass = SelectorClassFinder.Succeeded() ? SelectorClassFinder.Class : nullptr;



}

void AExSimPlayer::activateFunction() 
{
	//some action
	UE_LOG(LogTemp, Warning, TEXT("Activate function!"));
	//DataStorage->registerCmdToSelected(0,0.);
}

void AExSimPlayer::activateDifFunction() 
{
	//DataStorage->registerCmdToSelected(3,StartImpulse);
	if (OptionClass && ButtonClass && SelectorClass)
	{
		TArray<FString> options;
		options.Add("some");
		options.Add("give");
		options.Add("me");
		options.Add("reason");

		TargetWidget->addOptionToStorage("Test", "0.0; 0.0; 0.0");
		TargetWidget->addOptionToStorage("Empty", "0.0; 0.0; 0.0");
		TargetWidget->addOptionToStorage("gen", "0.0; 0.0; 0.0");

		TargetWidget->addSelectToStorage("example:", options);

		TargetWidget->addButtonToStorage("ok");

	}
}

void AExSimPlayer::moveRight(float value) 
{
	if (value != 0.f)
		moveAction(FVector(0,MoveHorizontalStepSz,0), FRotator(0,0,0));
}

void AExSimPlayer::moveLeft(float value) 
{
	if (value != 0.f)
		moveAction(FVector(0,-MoveHorizontalStepSz,0), FRotator(0,0,0));
}

void AExSimPlayer::moveAction(FVector loc, FRotator rot)
{
	if (DataStorage->getMode() == AExSimStorage::es_modes::EDIT)
	{
		DataStorage->manipulateGenerator(loc, rot);
	}
	else if (DataStorage->getMode() == AExSimStorage::es_modes::MOVE)
	{
		FRotator rotation = GetActorRotation() + rot;
		FVector direction = rotation.RotateVector(loc);
		SetActorLocation(GetActorLocation() + direction);
		SetActorRotation(rotation);
	}	
}

void AExSimPlayer::toDebugWindow(FString text)
{
	if (TargetWidget && TargetWidget->DebugText)
		TargetWidget->DebugText->SetText(FText::FromString(text));
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
	if (value != 0.f)
		moveAction(FVector(0,0,0), FRotator(0,RotateStepSZ,0));
}

void AExSimPlayer::rotateDown(float value) 
{
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


void AExSimPlayer::touchActor(AActor* actor, FVector mouse_loc, FVector hit_loc)
{
	FString output = "";
	if (DataStorage)
	{
		if (DataStorage->touchActor(actor, output))
		{
			ActorDragInitDist = hit_loc - mouse_loc;
			ActorDragDistance = ActorDragInitDist.Size();
			DataStorage->pickActor(actor, hit_loc);
			//TargetWidget->DebugText->SetText(FText::FromString(output));
			toDebugWindow(output);
		}
	}
}

void AExSimPlayer::moveActor(FVector mouse_loc, FVector mouse_dir)
{
	if (DataStorage)
	{
		FVector direction = mouse_dir * ActorDragDistance;
		DataStorage->moveActor(mouse_loc + direction);
		FVector loc;
		if (DataStorage->getActorInfo(loc))
			toDebugWindow(loc.ToString());
	}
}

void AExSimPlayer::releaseActor()
{
	if (DataStorage)
		DataStorage->letActor();
}

void AExSimPlayer::editActor(AActor* actor, FVector2D mouse_on_screen, FVector mouse_loc, FVector hit_loc)
{
	if(TargetWidget)
	{
		TargetWidget->setupConstrainOptions(mouse_on_screen, actor);
	}
}



void AExSimPlayer::setConstraintOptions()
{
	{
		toDebugWindow("Parent or Target is don't exist");
	}
	toDebugWindow("Setup constraint");
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

	if (TargetWidget && OptionClass && ButtonClass && SelectorClass)
	{
		TargetWidget->setOptionClass(OptionClass);
		TargetWidget->setButtonClass(ButtonClass);
		TargetWidget->setSelectorClass(SelectorClass);
		
		/*TArray<FString> options;
		options.Add("some");
		options.Add("give");
		options.Add("me");
		options.Add("reason");

		TargetWidget->addOptionToStorage("Test", "0.0; 0.0; 0.0");
		TargetWidget->addOptionToStorage("Empty", "0.0; 0.0; 0.0");
		TargetWidget->addOptionToStorage("gen", "0.0; 0.0; 0.0");

		TargetWidget->addSelectToStorage("example:", options);

		TargetWidget->addButtonToStorage("ok");*/

	}
	
	
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

