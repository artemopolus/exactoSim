// Fill out your copyright notice in the Description page of Project Settings.


#include "ExSelector.h"

void UExSelector::setSelectorText(FString name)
{
	SelectorText->SetText(FText::FromString(name));
}

void UExSelector::addSelectorValue(FString text)
{
	SelectorCombo->AddOption(text);
	if (SelectorCombo->GetOptionCount() == 1)
		SelectorCombo->OnSelectionChanged.AddDynamic(this,&UExSelector::onSelectorValueChanged);
}

void UExSelector::init(FString name, int id)
{
	setSelectorText(name);
	Id = id;
}

void UExSelector::onSelectorValueChanged(FString name, ESelectInfo::Type type)
{
	if(EventOnSelectorValueChanged.IsBound())
	{
		EventOnSelectorValueChanged.Broadcast(name, type, Id);
	}
}
