// Fill out your copyright notice in the Description page of Project Settings.


#include "ExEditableWidget.h"

#include "Components/TextBlock.h"


void UExEditableWidget::onTextCommitedRegistered(
	const FText & text, ETextCommit::Type type
	)
{
	FString out = TEXT("TextChanged: ") + text.ToString();
	if (type == ETextCommit::Default)
		out += "[Default]";
	else if (type == ETextCommit::Type::OnCleared)
		out += "[OnCleared]";
	else if (type == ETextCommit::Type::OnEnter)
		out += "[OnEnter]";
	else if (type == ETextCommit::Type::OnUserMovedFocus)
		out += "[OnUserMovedFocus]";

	if (EventOnTextCommit.IsBound())
	{
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

