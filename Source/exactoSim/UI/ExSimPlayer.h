// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExSimMainWidget.h"
#include "Camera/CameraComponent.h"
#include "exactoSim/Common/ExSimStorage.h"
#include "GameFramework/Pawn.h"
#include "ExSimPlayer.generated.h"

UCLASS()
class EXACTOSIM_API AExSimPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AExSimPlayer();




	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AExSimStorage * DataStorage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UExSimMainWidget * TargetWidget;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StartImpulse = -300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MoveVerticalStepSz = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MoveHorizontalStepSz = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RotateStepSZ = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent * TrgCamera;


private:
	UClass * OptionClass;
	UClass * ButtonClass;
	UClass * SelectorClass;
	UClass * ComboClass;

	FVector ActorDragInitDist;
	float ActorDragDistance;
	FRotator ActorDragInitRot;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
		void sendDataToStorage();

	//actions
	void activateFunction() ;
	void activateDifFunction() ;
	void undoFunction();
	//axis
	void moveRight(float value) ;
	void moveLeft(float value) ;
	void moveForward(float value);
	void moveBack(float value);
	void moveUp(float value);
	void moveDown(float value);
	void rotateUp(float value) ;
	void rotateDown(float value) ;
	void rotateRight(float value);
	void rotateLeft(float value);

	void selectActor(AActor * actor, FVector mouse_loc, FVector hit_loc);

	bool touchActor(AActor * actor, FVector mouse_loc, FVector hit_loc);
	void moveActor(FVector mouse_loc, FVector mouse_dir);
	void releaseActor();


	void editActor(AActor * actor, FVector2D mouse_on_screen, FVector mouse_loc, FVector hit_loc);
	void setConstraintOptions();

	void clickOnClearSpace();

private:
	void moveAction(FVector loc, FRotator rot);
	void toDebugWindow(FString text);
};
