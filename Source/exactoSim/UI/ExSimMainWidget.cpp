// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSimMainWidget.h"

#include "Components/TextBlock.h"

void UExSimMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Initialize();
}

bool UExSimMainWidget::Initialize()
{
	if (DebugText)
	{
		UE_LOG(LogTemp, Warning, TEXT("Debug text is inited"));

		DebugText->SetText(FText::FromString("Debug info:"));
	}
	return Super::Initialize();
}
