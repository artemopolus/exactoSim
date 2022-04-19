// Fill out your copyright notice in the Description page of Project Settings.


#include "ExEditableWidget.h"


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


	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, out);
}

void UExEditableWidget::initEditable( int pt)
{
	PtType = pt;
	ValueText->OnTextCommitted.AddDynamic(this,&UExEditableWidget::onTextCommitedRegistered);
}
