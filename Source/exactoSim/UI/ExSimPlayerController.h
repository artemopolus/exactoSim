// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExSimHUD.h"
#include "ExSimPlayer.h"
#include "GameFramework/PlayerController.h"

#include "ExSimPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EXACTOSIM_API AExSimPlayerController : public APlayerController
{
	GENERATED_BODY()

	AExSimPlayerController();
public:

	AExSimPlayer * PlayerPtr = nullptr;

	AExSimHUD * HUDPtr = nullptr;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void mouseLClick();
};
