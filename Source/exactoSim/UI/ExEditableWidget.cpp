// Fill out your copyright notice in the Description page of Project Settings.


#include "ExEditableWidget.h"

#include "Components/TextBlock.h"
#include "exactoSim/DataTypes/FExConstraintParams.h"
#include "exactoSim/Utils/ExConvert.h"


void UExEditableWidget::onTextCommitedRegistered(
	const FText & text, ETextCommit::Type type
	)
{
	if (text.ToString() == InitValue)
		return;
	FString out = TEXT("TextChanged: ") + text.ToString();
	if (type == ETextCommit::Default)
		out += "[Default]";
	else if (type == ETextCommit::Type::OnCleared)
		out += "[OnCleared]";
	else if (type == ETextCommit::Type::OnEnter)
		out += "[OnEnter]";
	else if (type == ETextCommit::Type::OnUserMovedFocus)
		out += "[OnUserMovedFocus]";

	if (EventOnTextCommit.IsBound()&&(
		(type == ETextCommit::Type::OnUserMovedFocus)||
		(type == ETextCommit::Type::OnEnter)
		))
	{
		EnExConstraintParamNames p = static_cast<EnExConstraintParamNames>(PtType);
		if (p > EnExConstraintParamNames::vector_start && p < EnExConstraintParamNames::string_start &&!ExConvert::checkVecStr(text.ToString()))
		{
			ValueText->SetText(FText::FromString(InitValue));
			return;
		}
		EventOnTextCommit.Broadcast(InitValue, text.ToString(), PtId, PtType);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, out);
}

void UExEditableWidget::initEditable(FString name, FString value, int id, int type)
{
	ValueName->SetText(FText::FromString(name));
	ValueText->SetText(FText::FromString(value));
	InitValue = value;
	PtType = type;
	PtId = id;
	ValueText->OnTextCommitted.AddDynamic(this,&UExEditableWidget::onTextCommitedRegistered);
}

void UExEditableWidget::update(FString value)
{
	InitValue = value;
	ValueText->SetText(FText::FromString(value));
}

FString UExEditableWidget::getName() const
{
	return ValueName->GetText().ToString();
}

FString UExEditableWidget::getValue() const
{
	return ValueText->GetText().ToString();
}

