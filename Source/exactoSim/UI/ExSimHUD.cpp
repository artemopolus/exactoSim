// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSimHUD.h"

FVector2D AExSimHUD::getMousePosition()
{
	float x = 0, y = 0;
	GetOwningPlayerController()->GetMousePosition(x,y);
	
	return FVector2D(x,y);
}

void AExSimHUD::getMouseProjection(FVector & loc, FVector & dir)
{
	GetOwningPlayerController()->DeprojectMousePositionToWorld(loc,dir);
}
