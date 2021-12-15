// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSelector.h"

void UExSelector::setSelectorText(FString name)
{
	SelectorText->SetText(FText::FromString(name));
}

void UExSelector::addSelectorValue(FString text)
{
	SelectorCombo->AddOption(text);
}
