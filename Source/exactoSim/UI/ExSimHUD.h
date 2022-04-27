// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ExSimHUD.generated.h"

/**
 * 
 */
UCLASS()
class EXACTOSIM_API AExSimHUD : public AHUD
{
	GENERATED_BODY()

public:
	FVector2D getMousePosition();
	void getMouseProjection(FVector & loc, FVector & dir);
	
};
