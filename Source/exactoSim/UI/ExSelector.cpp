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
	{
		setCurrentValue(text);
		SelectorCombo->OnSelectionChanged.AddDynamic(this,&UExSelector::onSelectorValueChanged);
	}
}

void UExSelector::setCurrentValue(FString trg)
{
	SelectorCombo->SetSelectedOption(trg);
	InitValue = trg;
}

void UExSelector::init(FString name, int32 id, int32 type)
{
	setSelectorText(name);
	PtId = id;
	PtType = type;
}

void UExSelector::onSelectorValueChanged(FString name, ESelectInfo::Type type)
{
	if(EventOnSelectorValueChanged.IsBound())
	{
		EventOnSelectorValueChanged.Broadcast(InitValue, name, PtType, PtId, static_cast<int32>(type));
	}
}
