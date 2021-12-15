// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "ExEditableWidget.generated.h"

/**
 * 
 */
UCLASS()
class EXACTOSIM_API UExEditableWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * ValueName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UEditableText * ValueText;
};
