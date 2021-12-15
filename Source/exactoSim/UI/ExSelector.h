// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "ExSelector.generated.h"

/**
 * 
 */
UCLASS()
class EXACTOSIM_API UExSelector : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UComboBoxString * SelectorCombo;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock * SelectorText;

	void setSelectorText(FString name);
	void addSelectorValue(FString text);
	
};
