// Fill out your copyright notice in the Description page of Project Settings.


#include "ExButtonWidget.h"

#include "Components/TextBlock.h"

void UExButtonWidget::setName(FString name)
{
	if (ButtonName)
	{
		ButtonName->SetText(FText::FromString(name));
		Name = name;
	}
}

FString UExButtonWidget::getButtonName()
{
	return Name;
}
