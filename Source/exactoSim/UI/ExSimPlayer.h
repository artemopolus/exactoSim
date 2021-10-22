// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	void activateFunction() ;
	void activateDifFunction() ;
	void moveRight() ;
	void moveLeft() ;
	void rotateUp() ;
	void rotateDown() ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AExSimStorage * DataStorage;
	UFUNCTION(BlueprintCallable)
		void sendDataToStorage();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float StartImpulse = -300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MoveHorizontalStepSz = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float RotateStepSZ = 10;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
